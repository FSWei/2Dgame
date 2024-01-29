#include "def.h"
#include "player.h"
#include "enemy.h"
#include "map.h"
#include "camera.h"
#include "input_handler.h"

#include "game.h"

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

	Game game;

	while (playAgain)
	{
		CLEARWINDOW;

		MOVECURSOR(0, 0);

#ifdef _WIN32
		HideConsoleCursor(); // 隐藏光标
#elif __linux__
		nodelay(stdscr, TRUE); // 设置非阻塞模式
#endif

		game.run();

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