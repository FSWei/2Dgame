#include "game.h"

Game::Game() : camera(nullptr), map(nullptr), background(nullptr), player(nullptr), inputhandler(nullptr), enemy(nullptr)
{
}

void Game::run()
{
	init();

#ifdef __linux__
    std::thread keyboardThread([this]() { keyboardCheck(); });
#endif
    std::thread inputThread([this]() { Input(inputhandler, player); });
    std::thread playerThread([this]() { updatePlayer(player); });
    std::thread enemyThread([this]() { updateEnemy(&enemy); });
    std::thread renderThread([this]() { render(player, &enemy, camera); });
    std::thread timerThread([this]() { timer(player); });

#ifdef __linux__
	keyboardThread.join();
#endif
	inputThread.join();
	playerThread.join();
	enemyThread.join();
	renderThread.join();
	timerThread.join();

	end();
}

void Game::init()
{
	camera = new Camera();
	map = new Map();
	background = new Background();
	player = new Player();
	inputhandler = new Inputhandler();
	enemy = new Enemy[MAXENEMY]{{11, 24}, {17, 24}, {16, 20}, {21, 15}};

	(*player).map = map;
	(*player).background = background;
	(*player).camera = camera;

	(*map).player = player;
	(*map).background = background;

	(*camera).map = map;

	for (int i = 0; i < MAXENEMY; i++)
	{
		enemy[i].map = map;
		enemy[i].background = background;
		enemy[i].camera = camera;
	}

	(*camera).init((*player).roleblock[0].pos.x - CAMERA_LENGTH / 2, (*player).roleblock[0].pos.y - CAMERA_HEIGHT / 2 + 2, CAMERA_LENGTH, CAMERA_HEIGHT);
	// (*camera).init((*player).pos.x - CAMERA_LENGTH / 2, (*player).pos.y - CAMERA_HEIGHT / 2 + 2, CAMERA_LENGTH, CAMERA_HEIGHT);
	(*map).init();
	(*camera).showBorder();
	(*camera).camera_render();
	(*player).render();
}

#ifdef __linux__
void *Game::keyboardCheck()
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

void *Game::Input(Inputhandler *ih, Player *player)
{
	while (gamestatus)
	{
		ih->handleInput(*player);
		usleep(100000);
	}
	return NULL;
}

void *Game::updatePlayer(Player *player)
{
	while (gamestatus)
	{
		player->update();
		player->camera->update(player->roleblock[0].pos.x, player->roleblock[0].pos.y); // 既然摄像头是跟着player移动的，那就让它和player一起更新

		usleep(player->updatetime);
	}
	return NULL;
}

void *Game::updateEnemy(Enemy (**e))
{
	while (gamestatus)
	{
		for (int i = 0; i < MAXENEMY; i++)
		{
			(*e)[i].update();
		}
		usleep((*e)[0].updatetime);
	}
	return NULL;
}

void *Game::render(Player *player, Enemy (**e), Camera *camera)
{
	while (gamestatus)
	{
		mtx.lock();
		print_data(player, e); // 打印数据
		mtx.unlock();

		mtx.lock();
		camera->camera_render();
		mtx.unlock();

		mtx.lock();
		player->render();
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

		usleep(player->camera->updatetime);
	}

	return NULL;
}

void Game::print_data(Player *player, Enemy (**e))
{
	SetColor(_255_255_255, _0_0_0);

	int tmp = 0;

	// MOVECURSOR(LEFT_BORDER * 2, TOP_BORDER + CAMERA_HEIGHT + BOTTOM_BORDER + tmp++);
	// PRINT("preblock:%c   ", player->preblock);
	MOVECURSOR(LEFT_BORDER * 2, TOP_BORDER + CAMERA_HEIGHT + BOTTOM_BORDER + tmp++);
	PRINT("[0]pos:%d/%d , [1]pos:%d/%d   ", player->roleblock[0].pos.x, player->roleblock[0].pos.y, player->roleblock[1].pos.x, player->roleblock[1].pos.y);
	// MOVECURSOR(LEFT_BORDER * 2, TOP_BORDER + CAMERA_HEIGHT + BOTTOM_BORDER + tmp++);
	// PRINT("next:%d,%d:%c    ", player->pos.x + player->v.x * player->xdirection, player->pos.y + player->isAirborne * player->ydirection, player->map->blocktype[player->pos.y + player->isAirborne * player->ydirection][player->pos.x + player->v.x * player->xdirection].type);
	MOVECURSOR(LEFT_BORDER * 2, TOP_BORDER + CAMERA_HEIGHT + BOTTOM_BORDER + tmp++);
	PRINT("v:%d,%d   ", player->v.x, player->v.y);
	MOVECURSOR(LEFT_BORDER * 2, TOP_BORDER + CAMERA_HEIGHT + BOTTOM_BORDER + tmp++);
	PRINT("direction:%d,%d   ", player->xdirection, player->ydirection);
	MOVECURSOR(LEFT_BORDER * 2, TOP_BORDER + CAMERA_HEIGHT + BOTTOM_BORDER + tmp++);
	PRINT("jump_ticks:%d   ", player->jump_ticks);
	MOVECURSOR(LEFT_BORDER * 2, TOP_BORDER + CAMERA_HEIGHT + BOTTOM_BORDER + tmp++);
	PRINT("canJump:%d,canFall:%d,isAirborne:%d,isJumping:%d,isWalking:%d,isdash:%d     ", player->canJump, player->canFall, player->isAirborne, player->isJumping, player->isWalking, player->isdash);
	// PRINT("canJump1:%d,canJump2:%d,isAirborne :%d,isJumping:%d,isWalking:%d,isdash:%d     ", player->canJump1, player->canJump2, player->isAirborne, player->isJumping, player->isWalking, player->isdash);
	MOVECURSOR(LEFT_BORDER * 2, TOP_BORDER + CAMERA_HEIGHT + BOTTOM_BORDER + tmp++);
	PRINT("time:%d   ", _time);
	MOVECURSOR(LEFT_BORDER * 2, TOP_BORDER + CAMERA_HEIGHT + BOTTOM_BORDER + tmp++);
	PRINT("c.left:%d,c.top:%d    ", player->camera->left, player->camera->top);
	MOVECURSOR(LEFT_BORDER * 2, TOP_BORDER + CAMERA_HEIGHT + BOTTOM_BORDER + tmp++);
	PRINT("dash_cooling_time:%d   ", player->dash_cooling_time);
	MOVECURSOR(LEFT_BORDER * 2, TOP_BORDER + CAMERA_HEIGHT + BOTTOM_BORDER + tmp++);
	// 打印player下一个位置的方块类型
	PRINT("[0]next:%d,%d:%c    ", player->roleblock[0].pos.x + player->v.x * player->xdirection, player->roleblock[0].pos.y + player->isAirborne * player->ydirection, player->map->blocktype[player->roleblock[0].pos.y + player->isAirborne * player->ydirection][player->roleblock[0].pos.x + player->v.x * player->xdirection].type);
	PRINT("[1]next:%d,%d:%c    ", player->roleblock[1].pos.x + player->v.x * player->xdirection, player->roleblock[1].pos.y + player->isAirborne * player->ydirection, player->map->blocktype[player->roleblock[1].pos.y + player->isAirborne * player->ydirection][player->roleblock[1].pos.x + player->v.x * player->xdirection].type);
	PRINT("[2]next:%d,%d:%c    ", player->roleblock[1].pos.x + player->v.x * player->xdirection, player->roleblock[1].pos.y + player->isAirborne * player->ydirection, player->map->blocktype[player->roleblock[1].pos.y + player->isAirborne * player->ydirection][player->roleblock[1].pos.x + player->v.x * player->xdirection].type);
	PRINT("[3]next:%d,%d:%c    ", player->roleblock[1].pos.x + player->v.x * player->xdirection, player->roleblock[1].pos.y + player->isAirborne * player->ydirection, player->map->blocktype[player->roleblock[1].pos.y + player->isAirborne * player->ydirection][player->roleblock[1].pos.x + player->v.x * player->xdirection].type);
	MOVECURSOR(LEFT_BORDER * 2, TOP_BORDER + CAMERA_HEIGHT + BOTTOM_BORDER + tmp++);
	//打印preblock
	PRINT("[0]preblock:%c   ", player->roleblock[0].preblock);
	PRINT("[1]preblock:%c   ", player->roleblock[1].preblock);
	PRINT("[2]preblock:%c   ", player->roleblock[2].preblock);
	PRINT("[3]preblock:%c   ", player->roleblock[3].preblock);

	for (int i = 0; i < MAXENEMY; i++)
	{
		MOVECURSOR((LEFT_BORDER + CAMERA_LENGTH + RIGHT_BORDER) * 2, TOP_BORDER + i);
		PRINT("enemy%d.pos%d,%d    ", i, (*e)[i].pos.x, (*e)[i].pos.y);
	}
	UnsetColor(_255_255_255, _0_0_0);
}

void *Game::timer(Player *player) // 计时器
{
	while (gamestatus)
	{
		_time++;
		player->skillcool();
		usleep(1000000); // 一秒
	}
	return NULL;
}

void Game::end()
{
	delete camera;
	delete map;
	delete background;
	delete player;
	delete inputhandler;
	delete[] enemy;
}

void Game::win()
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

void Game::lose()
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
