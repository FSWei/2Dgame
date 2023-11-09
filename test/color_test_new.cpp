#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <thread>

// void tmp(int i)
// {
// 	for (int j = 0; j <= 255; j++)
// 		for (int k = 0; k <= 255; k++)
// 		{
// 			// printf("\x1B[38;2;%d;%d;%dm\x1B[48;2;%d;%d;%dm%d:%d:%d.\x1B[0m\n", i, j, k, k, j, i, i, j, k);
// 			printf("\x1B[38;2;%d;%d;%dm\x1B[48;2;%d;%d;%dm", i, j, k, k, j, i);
// 			printf("%d:%d:%d\n", i, j, k);
// 			printf("\x1B[0m");
// 		}
// }

// int main()
// {
// 	int num = 255;
// 	std::thread t[num + 1];
// 	for (int i = 0; i <= num; i++)
// 	{
// 		t[i] = std::thread(tmp, i);
// 	}
// 	for (int i = 0; i <= num; i++)
// 	{
// 		t[i].join();
// 	}

// 	system("pause");
// 	return 0;
// }

// int main()
// {
// 	const int ENABLE_VIRTUAL_TERMINAL_PROCESSING = 0x0004;
// 	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
// 	DWORD consoleMode;
// 	GetConsoleMode(hConsole, &consoleMode);
// 	consoleMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

// 	SetConsoleMode(hConsole, consoleMode);
// 	// 获取控制台屏幕缓冲区的信息
// 	CONSOLE_SCREEN_BUFFER_INFOEX csbi;
// 	csbi.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
// 	GetConsoleScreenBufferInfoEx(hConsole, &csbi);

// 	printf("%d\n", csbi.ColorTable[0]);
// 	// 修改颜色表中的颜色值
// 	csbi.ColorTable[0] = RGB(255, 165, 188);
// 	csbi.ColorTable[1] = RGB(255, 165, 188);
// 	csbi.ColorTable[2] = RGB(255, 165, 188);
// 	csbi.ColorTable[3] = RGB(255, 165, 188);
// 	csbi.ColorTable[4] = RGB(255, 165, 188);
// 	csbi.ColorTable[5] = RGB(255, 165, 188);
// 	csbi.ColorTable[6] = RGB(255, 165, 188);
// 	csbi.ColorTable[7] = RGB(255, 165, 188);
// 	csbi.ColorTable[8] = RGB(255, 165, 188);
// 	csbi.ColorTable[9] = RGB(255, 165, 188);
// 	csbi.ColorTable[10] = RGB(255, 165, 188);
// 	csbi.ColorTable[11] = RGB(255, 165, 188);
// 	csbi.ColorTable[12] = RGB(255, 165, 188);
// 	csbi.ColorTable[13] = RGB(255, 165, 188);
// 	csbi.ColorTable[14] = RGB(255, 165, 188);
// 	csbi.ColorTable[15] = RGB(255, 165, 188);

// 	// 应用修改后的颜色表和文本属性
// 	SetConsoleScreenBufferInfoEx(hConsole, &csbi);

// 	printf("%d\n", csbi.ColorTable[0]);
// 	printf("%d\n", csbi.wAttributes);

// 	SetConsoleTextAttribute(hConsole, csbi.wAttributes);

// 	// 输出使用新颜色的文本
// 	std::cout << "AAAAAAAAAAA\n";
// 	system("pause");
// 	return 0;
// }

// #include <iostream>
// #include <Windows.h>

// int main()
// {
// 	const int ENABLE_VIRTUAL_TERMINAL_PROCESSING = 0x0004;
// 	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

// 	DWORD consoleMode;
// 	if (GetConsoleMode(hConsole, &consoleMode))
// 	{
// 		// 启用 ENABLE_VIRTUAL_TERMINAL_PROCESSING 标志
// 		consoleMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

// 		// 设置新的控制台模式
// 		if (SetConsoleMode(hConsole, consoleMode))
// 		{
// 			std::cout << "enable ENABLE_VIRTUAL_TERMINAL_PROCESSING。" << std::endl;

// 			// 现在你可以在控制台中使用 ANSI 转义码来控制文本属性和颜色
// 			std::cout << "\x1B[31mThis is red text.\x1B[0m" << std::endl;
// 		}
// 		else
// 		{
// 			std::cerr << "cannot set:" << GetLastError() << std::endl;
// 		}
// 	}
// 	else
// 	{
// 		std::cerr << "cannot get：" << GetLastError() << std::endl;
// 	}

// 	system("pause");
// 	return 0;
// }

int main()
{
    printf("\x1B[38;2;%d;%d;%dm\x1B[48;2;%d;%d;%dm", 001, 001, 001, 255, 255, 255);
    char s[32 * 16 * 42] = "\x1B[38;2;001;001;001m\x1B[48;2;255;255;255m";
    printf("%d\n", strlen(s));
    printf("%d\n", sizeof(s));
    printf("%d:%d:%d\n", 1, 2, 3);
    printf("\x1B[0m");
    system("pause");
}