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
	// system("cls");
	clear();
	for (int i = 0; i < 11; i++)
	{
		for (int j = 0; j < 32; j++)
		{
			if (j == 0 || j == 31 || i == 0 || i == 10)
			{
				printw("* ");
				// Sleep(10);
				napms(10);
			}
			else
				printw("  ");
			if (i == 5 && j == 12)
			{
				printw("YOU WIN ");
				j = 16;
			}
		}
		printw("\n");
	}
	UnsetColor(_255_255_255, _0_0_0);
}

void lose()
{
	SetColor(_255_255_255, _0_0_0);
	// system("cls");
	clear();
	for (int i = 0; i < 11; i++)
	{
		for (int j = 0; j < 32; j++)
		{
			if (j == 0 || j == 31 || i == 0 || i == 10)
			{
				printw("* ");
				// Sleep(10);
				napms(10);
			}
			else
				printw("  ");
			if (i == 5 && j == 12)
			{
				printw("YOU LOSE");
				j = 16;
			}
		}
		printw("\n");
	}
	UnsetColor(_255_255_255, _0_0_0);
}

void print_data(player *p, enemy (*e)[MAXENEMY])
{
	SetColor(_255_255_255, _0_0_0);

	// int tmp = 0;

	// gotoxy(LEFT_BORDER * 2, TOP_BORDER + CAMERA_HEIGHT + BOTTOM_BORDER + tmp++);
	// printw("preblock:%c   ", p->preblock);
	// gotoxy(LEFT_BORDER * 2, TOP_BORDER + CAMERA_HEIGHT + BOTTOM_BORDER + tmp++);
	// printw("pos:%d,%d   ", p->pos.x, p->pos.y);
	// gotoxy(LEFT_BORDER * 2, TOP_BORDER + CAMERA_HEIGHT + BOTTOM_BORDER + tmp++);
	// printw("next:%d,%d:%c    ", p->pos.x + p->v.x * p->xdirection, p->pos.y + p->isair * p->ydirection, p->m->blocktype[p->pos.y + p->isair * p->ydirection][p->pos.x + p->v.x * p->xdirection].type);
	// gotoxy(LEFT_BORDER * 2, TOP_BORDER + CAMERA_HEIGHT + BOTTOM_BORDER + tmp++);
	// printw("v:%d,%d   ", p->v.x, p->v.y);
	// gotoxy(LEFT_BORDER * 2, TOP_BORDER + CAMERA_HEIGHT + BOTTOM_BORDER + tmp++);
	// printw("direction:%d,%d   ", p->xdirection, p->ydirection);
	// gotoxy(LEFT_BORDER * 2, TOP_BORDER + CAMERA_HEIGHT + BOTTOM_BORDER + tmp++);
	// printw("hop_count:%d   ", p->hop_count);
	// gotoxy(LEFT_BORDER * 2, TOP_BORDER + CAMERA_HEIGHT + BOTTOM_BORDER + tmp++);
	// printw("istop:%d,isair:%d,isjump:%d,iswalk:%d,isdash:%d     ", p->istop, p->isair, p->isjump, p->iswalk, p->isdash);
	// gotoxy(LEFT_BORDER * 2, TOP_BORDER + CAMERA_HEIGHT + BOTTOM_BORDER + tmp++);
	// printw("time:%d   ", _time);
	// gotoxy(LEFT_BORDER * 2, TOP_BORDER + CAMERA_HEIGHT + BOTTOM_BORDER + tmp++);
	// printw("c.left:%d,c.top:%d    ", p->c->left, p->c->top);
	// gotoxy(LEFT_BORDER * 2, TOP_BORDER + CAMERA_HEIGHT + BOTTOM_BORDER + tmp++);
	// printw("dash_cooling_time:%d   ", p->dash_cooling_time);

	int tmp = 0;

	move(TOP_BORDER + CAMERA_HEIGHT + BOTTOM_BORDER + tmp++, LEFT_BORDER * 2);
	printw("preblock:%c   ", p->preblock);

	move(TOP_BORDER + CAMERA_HEIGHT + BOTTOM_BORDER + tmp++, LEFT_BORDER * 2);
	printw("pos:%d,%d   ", p->pos.x, p->pos.y);

	move(TOP_BORDER + CAMERA_HEIGHT + BOTTOM_BORDER + tmp++, LEFT_BORDER * 2);
	printw("next:%d,%d:%c    ", p->pos.x + p->v.x * p->xdirection, p->pos.y + p->isair * p->ydirection, p->m->blocktype[p->pos.y + p->isair * p->ydirection][p->pos.x + p->v.x * p->xdirection].type);

	move(TOP_BORDER + CAMERA_HEIGHT + BOTTOM_BORDER + tmp++, LEFT_BORDER * 2);
	printw("v:%d,%d   ", p->v.x, p->v.y);

	move(TOP_BORDER + CAMERA_HEIGHT + BOTTOM_BORDER + tmp++, LEFT_BORDER * 2);
	printw("direction:%d,%d   ", p->xdirection, p->ydirection);

	move(TOP_BORDER + CAMERA_HEIGHT + BOTTOM_BORDER + tmp++, LEFT_BORDER * 2);
	printw("hop_count:%d   ", p->hop_count);

	move(TOP_BORDER + CAMERA_HEIGHT + BOTTOM_BORDER + tmp++, LEFT_BORDER * 2);
	printw("istop:%d,isair:%d,isjump:%d,iswalk:%d,isdash:%d     ", p->istop, p->isair, p->isjump, p->iswalk, p->isdash);

	move(TOP_BORDER + CAMERA_HEIGHT + BOTTOM_BORDER + tmp++, LEFT_BORDER * 2);
	printw("time:%d,ch:%d\\%c   ", _time, ch, ch);

	move(TOP_BORDER + CAMERA_HEIGHT + BOTTOM_BORDER + tmp++, LEFT_BORDER * 2);
	printw("c.left:%d,c.top:%d    ", p->c->left, p->c->top);

	move(TOP_BORDER + CAMERA_HEIGHT + BOTTOM_BORDER + tmp++, LEFT_BORDER * 2);
	printw("dash_cooling_time:%d   ", p->dash_cooling_time);

	for (int i = 0; i < MAXENEMY; i++)
	{
		// gotoxy((LEFT_BORDER + CAMERA_LENGTH + RIGHT_BORDER) * 2, TOP_BORDER + i);
		// printw("enemy%d.pos%d,%d    ", i, (*e)[i].pos.x, (*e)[i].pos.y);
		move(TOP_BORDER + i, (LEFT_BORDER + CAMERA_LENGTH + RIGHT_BORDER) * 2);
		printw("enemy%d.pos%d,%d    ", i, (*e)[i].pos.x, (*e)[i].pos.y);
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
		}
		else if (iswin == 0)
		{
			gamestatus = 0;
			lose();
		}

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

	std::thread playerThread(updatePlayer, &p);
	std::thread enemyThread(updateEnemy, &e);
	std::thread renderThread(render, &p, &e);
	std::thread timerThread(timer, &p);

	playerThread.join();
	enemyThread.join();
	renderThread.join();
	timerThread.join();
}

int main()
{
	initscr(); // 初始化ncurses
	start_color();
	cbreak();			   // 禁用行缓冲
	noecho();			   // 不在屏幕上显示按键
	nodelay(stdscr, TRUE); // 设置非阻塞模式
	keypad(stdscr, TRUE);  // 启用键盘特殊按键（如方向键）

	InitAllcolor();

	bool playAgain = true;

	while (playAgain)
	{
		// system("cls");
		clear();
		// gotoxy(0, 0);
		move(0, 0);
		// HideConsoleCursor(); // 隐藏光标

		nodelay(stdscr, TRUE); // 设置非阻塞模式

		test();

		gamestatus = 1;
		// ShowConsoleCursor();//显示光标

		printw("Game over! Play again? (Y/N): ");

		// FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE)); // 清空输入缓冲区

		char choice = 0;
		// scanf(" %c", &choice); //%c前面加空格以跳过制表符、空格和换行符等空白字符，确保scanf只读取有效的字符

		nodelay(stdscr, FALSE); // 设置阻塞模式
		choice = getch();
		if (choice == 'N' || choice == 'n')
		{
			playAgain = false;
		}
	}

	endwin();

	return 0;
}