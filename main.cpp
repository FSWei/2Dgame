#include "def.h"
#include "player.h"
#include "enemy.h"
#include "map.h"
#include "camera.h"

int iswin = -1;
int gamestatus = 1; // 0是结束，1是正常状态
std::mutex mtx;		// 使用互斥锁，避免因为两者同时渲染而出现的光标混乱问题

void win()
{
	SetColor(_255_255_255, _0_0_0);
	CLEARWINDOW;

	for (int i = 0; i < 11; i++)
	{
		for (int j = 0; j < 32; j++)
		{
			if (j == 0 || j == 31 || i == 0 || i == 10)
			{
				PRINT("* ");

				usleep(10000);
			}
			else
				PRINT("  ");
			if (i == 5 && j == 12)
			{
				PRINT("YOU WIN ");
				j = 16;
			}
		}
		PRINT("\n");
	}
	UnsetColor(_255_255_255, _0_0_0);
}

void lose()
{
	SetColor(_255_255_255, _0_0_0);
	CLEARWINDOW;

	for (int i = 0; i < 11; i++)
	{
		for (int j = 0; j < 32; j++)
		{
			if (j == 0 || j == 31 || i == 0 || i == 10)
			{
				PRINT("* ");
				usleep(10000);
			}
			else
				PRINT("  ");
			if (i == 5 && j == 12)
			{
				PRINT("YOU LOSE");
				j = 16;
			}
		}
		PRINT("\n");
	}
	UnsetColor(_255_255_255, _0_0_0);
}

void print_data(player *p, enemy (*e)[MAXENEMY])
{
	SetColor(_255_255_255, _0_0_0);

	int tmp = 0;

	MOVECURSOR(LEFT_BORDER * 2, TOP_BORDER + CAMERA_HEIGHT + BOTTOM_BORDER + tmp++);
	PRINT("preblock:%c   ", p->preblock);
	MOVECURSOR(LEFT_BORDER * 2, TOP_BORDER + CAMERA_HEIGHT + BOTTOM_BORDER + tmp++);
	PRINT("pos:%d,%d   ", p->pos.x, p->pos.y);
	MOVECURSOR(LEFT_BORDER * 2, TOP_BORDER + CAMERA_HEIGHT + BOTTOM_BORDER + tmp++);
	PRINT("next:%d,%d:%c    ", p->pos.x + p->v.x * p->xdirection, p->pos.y + p->isair * p->ydirection, p->m->blocktype[p->pos.y + p->isair * p->ydirection][p->pos.x + p->v.x * p->xdirection].type);
	MOVECURSOR(LEFT_BORDER * 2, TOP_BORDER + CAMERA_HEIGHT + BOTTOM_BORDER + tmp++);
	PRINT("v:%d,%d   ", p->v.x, p->v.y);
	MOVECURSOR(LEFT_BORDER * 2, TOP_BORDER + CAMERA_HEIGHT + BOTTOM_BORDER + tmp++);
	PRINT("direction:%d,%d   ", p->xdirection, p->ydirection);
	MOVECURSOR(LEFT_BORDER * 2, TOP_BORDER + CAMERA_HEIGHT + BOTTOM_BORDER + tmp++);
	PRINT("hop_count:%d   ", p->hop_count);
	MOVECURSOR(LEFT_BORDER * 2, TOP_BORDER + CAMERA_HEIGHT + BOTTOM_BORDER + tmp++);
	PRINT("istop:%d,isair:%d,isjump:%d,iswalk:%d,isdash:%d     ", p->istop, p->isair, p->isjump, p->iswalk, p->isdash);
	MOVECURSOR(LEFT_BORDER * 2, TOP_BORDER + CAMERA_HEIGHT + BOTTOM_BORDER + tmp++);
	PRINT("time:%d   ", _time);
	MOVECURSOR(LEFT_BORDER * 2, TOP_BORDER + CAMERA_HEIGHT + BOTTOM_BORDER + tmp++);
	PRINT("c.left:%d,c.top:%d    ", p->c->left, p->c->top);
	MOVECURSOR(LEFT_BORDER * 2, TOP_BORDER + CAMERA_HEIGHT + BOTTOM_BORDER + tmp++);
	PRINT("dash_cooling_time:%d   ", p->dash_cooling_time);

	for (int i = 0; i < MAXENEMY; i++)
	{
		MOVECURSOR((LEFT_BORDER + CAMERA_LENGTH + RIGHT_BORDER) * 2, TOP_BORDER + i);
		PRINT("enemy%d.pos%d,%d    ", i, (*e)[i].pos.x, (*e)[i].pos.y);
	}
	UnsetColor(_255_255_255, _0_0_0);
}

void *updatePlayer(player *p)
{
	while (gamestatus)
	{
		p->input();
		iswin = p->update();
		p->c->update(p->pos.x, p->pos.y); // 既然摄像头是跟着player移动的，那就让它和player一起更新

		usleep(p->updatetime);
	}
	return NULL;
}

void *updateEnemy(enemy (*e)[MAXENEMY])
{
	while (gamestatus)
	{
		for (int i = 0; i < MAXENEMY; i++)
		{
			(*e)[i].update();
		}
		usleep(200000);
	}
	return NULL;
}

void *render(player *p, enemy (*e)[MAXENEMY])
{
	while (gamestatus)
	{
		mtx.lock();
		print_data(p, e); // 打印数据
		mtx.unlock();

		mtx.lock();
		p->camera_render();
		mtx.unlock();

		mtx.lock();
		p->render();
		mtx.unlock();

		for (int i = 0; i < MAXENEMY; i++)
		{
			mtx.lock();
			(*e)[i].render();
			mtx.unlock();
		}

		if (iswin == 1)
		{
			gamestatus = 0; // 游戏结束
			win();
			PRINT("Game over! Play again? (Y/N): ");
		}
		else if (iswin == 0)
		{
			gamestatus = 0;
			lose();
			PRINT("Game over! Play again? (Y/N): ");
		}
#ifdef __linux__
		refresh();
#endif

		usleep(p->c->updatetime);
	}

	return NULL;
}

void *timer(player *p) // 计时器
{
	while (gamestatus)
	{
		p->timer();
		usleep(1000000); // 一秒
	}
	return NULL;
}

#ifdef __linux__
void *keyboardCheck()
{
	// 打开第一个键盘设备
	int fd_keyboard1 = open(KEYBOARD_DEVICE_1, O_RDONLY);
	if (fd_keyboard1 == -1)
	{
		perror("open keyboard device 1 failed");
		return NULL;
	}

	// 打开第二个键盘设备
	int fd_keyboard2 = open(KEYBOARD_DEVICE_2, O_RDONLY);
	if (fd_keyboard2 == -1)
	{
		perror("open keyboard device 2 failed");
		close(fd_keyboard1);
		return NULL;
	}

	// 使用 select 实现同时检测键盘输入
	fd_set readfds;
	FD_ZERO(&readfds);
	FD_SET(fd_keyboard1, &readfds);
	FD_SET(fd_keyboard2, &readfds);
	int maxfd = (fd_keyboard1 > fd_keyboard2) ? fd_keyboard1 : fd_keyboard2;

	struct input_event ev;
	ssize_t ret;
	while (gamestatus)
	{
		ret = select(maxfd + 1, &readfds, NULL, NULL, NULL);
		if (ret == -1)
		{
			perror("select error");
			break;
		}
		else if (ret > 0)
		{
			if (FD_ISSET(fd_keyboard1, &readfds))
			{
				ret = read(fd_keyboard1, &ev, sizeof(ev));
				if (ret == sizeof(ev) && ev.type == EV_KEY)
				{
					if (ev.value == 1)
						key_state[ev.code] = 1; // 按键按下，状态设置为1
					else if (ev.value == 0)
						key_state[ev.code] = 0; // 按键释放，状态设置为0
				}
			}

			if (FD_ISSET(fd_keyboard2, &readfds))
			{
				ret = read(fd_keyboard2, &ev, sizeof(ev));
				if (ret == sizeof(ev) && ev.type == EV_KEY)
				{
					if (ev.value == 1)
						key_state[ev.code] = 1; // 按键按下，状态设置为1
					else if (ev.value == 0)
						key_state[ev.code] = 0; // 按键释放，状态设置为0
				}
			}
		}

		// 清空文件描述符集合
		FD_ZERO(&readfds);
		FD_SET(fd_keyboard1, &readfds);
		FD_SET(fd_keyboard2, &readfds);

		// usleep(1000);
	}

	// set all key_state to 0

	for (int i = 0; i < MAX_KEYCODE + 1; i++)
	{
		key_state[i] = 0;
	}

	// 关闭文件描述符
	close(fd_keyboard1);
	close(fd_keyboard2);

	return NULL;
}
#endif

void test()
{
	camera c;
	map m;
	background bg;
	player p;
	enemy e[MAXENEMY] = {{11, 24}, {17, 24}, {16, 21}, {21, 18}};
	// enemy e[MAXENEMY] = {{11, 24}};

	p.m = &m;
	p.bg = &bg;
	p.c = &c;

	m.p = &p;
	m.bg = &bg;

	for (int i = 0; i < MAXENEMY; i++) // enemy的指针需要用循环添加
	{
		m.e[i] = &e[i];

		p.e[i] = &e[i];

		e[i].m = &m;
		e[i].bg = &bg;
		e[i].c = &c;
	}

	c.init(p.pos.x - CAMERA_HEIGHT / 2, p.pos.y - CAMERA_LENGTH / 2 + 2, CAMERA_LENGTH, CAMERA_HEIGHT);

	m.init();

	c.showBorder();
	// m.show();
	// 展示初始camera图像
	// m.camera_show();

#ifdef __linux__
	std::thread keyboardThread(keyboardCheck);
#endif
	std::thread playerThread(updatePlayer, &p);
	std::thread enemyThread(updateEnemy, &e);
	std::thread renderThread(render, &p, &e);
	std::thread timerThread(timer, &p);

#ifdef __linux__
	keyboardThread.join();
#endif
	playerThread.join();
	enemyThread.join();
	renderThread.join();
	timerThread.join();
}

int main()
{
#ifdef __linux__
	initscr(); // 初始化ncurses
	start_color();
	cbreak();			   // 禁用行缓冲
	noecho();			   // 不在屏幕上显示按键
	nodelay(stdscr, TRUE); // 设置非阻塞模式
	keypad(stdscr, TRUE);  // 启用键盘特殊按键（如方向键）
#endif
	InitAllcolor();

	bool playAgain = true;

	while (playAgain)
	{
		CLEARWINDOW;

		MOVECURSOR(0, 0);

#ifdef _WIN32
		HideConsoleCursor(); // 隐藏光标
#elif __linux__
		nodelay(stdscr, TRUE); // 设置非阻塞模式
#endif

		test();

		gamestatus = 1;

#ifdef _WIN32
		ShowConsoleCursor();									 // 显示光标
		FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE)); // 清空输入缓冲区
#endif

		char choice = 0;
#ifdef _WIN32
		scanf(" %c", &choice); //%c前面加空格以跳过制表符、空格和换行符等空白字符，确保scanf只读取有效的字符
#elif __linux__
		nodelay(stdscr, FALSE); // 设置阻塞模式
		choice = getch();
#endif

		if (choice == 'N' || choice == 'n')
		{
			playAgain = false;
		}
		else if (choice == 'Y' || choice == 'y')
		{
			playAgain = true;
		}
	}

#ifdef __linux__
	endwin();
#endif

	return 0;
}