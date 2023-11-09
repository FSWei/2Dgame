#include <iostream>
#include <Windows.h>
#include <string>
#include <unistd.h>
#include <time.h>

const int bufferSize = 22561;
char consoleBuffer[bufferSize];
const char lenbuf[] = "\x1B[38;2;123;123;123m\x1B[48;2;123;123;123m. \x1B[0m\x1B[38;2;123;123;123m\x1B[48;2;123;123;123m. \x1B[0m\x1B[38;2;123;123;123m\x1B[48;2;123;123;123m. \x1B[0m\x1B[38;2;123;123;123m\x1B[48;2;123;123;123m. \x1B[0m\x1B[38;2;123;123;123m\x1B[48;2;123;123;123m. \x1B[0m\x1B[38;2;123;123;123m\x1B[48;2;123;123;123m. \x1B[0m\x1B[38;2;123;123;123m\x1B[48;2;123;123;123m. \x1B[0m\x1B[38;2;123;123;123m\x1B[48;2;123;123;123m. \x1B[0m\x1B[38;2;123;123;123m\x1B[48;2;123;123;123m. \x1B[0m\x1B[38;2;123;123;123m\x1B[48;2;123;123;123m. \x1B[0m\x1B[38;2;123;123;123m\x1B[48;2;123;123;123m. \x1B[0m\x1B[38;2;123;123;123m\x1B[48;2;123;123;123m. \x1B[0m\x1B[38;2;123;123;123m\x1B[48;2;123;123;123m. \x1B[0m\x1B[38;2;123;123;123m\x1B[48;2;123;123;123m. \x1B[0m\x1B[38;2;123;123;123m\x1B[48;2;123;123;123m. \x1B[0m\x1B[38;2;123;123;123m\x1B[48;2;123;123;123m. \x1B[0m\x1B[38;2;123;123;123m\x1B[48;2;123;123;123m. \x1B[0m\x1B[38;2;123;123;123m\x1B[48;2;123;123;123m. \x1B[0m\x1B[38;2;123;123;123m\x1B[48;2;123;123;123m. \x1B[0m\x1B[38;2;123;123;123m\x1B[48;2;123;123;123m. \x1B[0m\x1B[38;2;123;123;123m\x1B[48;2;123;123;123m. \x1B[0m\x1B[38;2;123;123;123m\x1B[48;2;123;123;123m. \x1B[0m\x1B[38;2;123;123;123m\x1B[48;2;123;123;123m. \x1B[0m\x1B[38;2;123;123;123m\x1B[48;2;123;123;123m. \x1B[0m\x1B[38;2;123;123;123m\x1B[48;2;123;123;123m. \x1B[0m\x1B[38;2;123;123;123m\x1B[48;2;123;123;123m. \x1B[0m\x1B[38;2;123;123;123m\x1B[48;2;123;123;123m. \x1B[0m\x1B[38;2;123;123;123m\x1B[48;2;123;123;123m. \x1B[0m\x1B[38;2;123;123;123m\x1B[48;2;123;123;123m. \x1B[0m\x1B[38;2;123;123;123m\x1B[48;2;123;123;123m. \x1B[0m\x1B[38;2;123;123;123m\x1B[48;2;123;123;123m. \x1B[0m\x1B[38;2;123;123;123m\x1B[48;2;123;123;123m. \x1B[0m\r\n";
const char tmp2buf[] = "................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................\r\n";
const char tmpbuf[] = "\x1B[38;2;123;123;123m\x1B[48;2;123;123;123m. \x1B[0m\x1B[38;2;123;123;123m\x1B[48;2;123;123;123m. \x1B[0m\x1B[38;2;123;123;123m\x1B[48;2;123;123;123m. \x1B[0m\x1B[38;2;123;123;123m\x1B[48;2;123;123;123m. \x1B[0m\x1B[38;2;123;123;123m\x1B[48;2;123;123;123m. \x1B[0m\x1B[38;2;123;123;123m\x1B[48;2;123;123;123m. \x1B[0m\x1B[38;2;123;123;123m\x1B[48;2;123;123;123m. \x1B[0m\r\n";
const char buf[] = "\x1B[38;2;123;123;123m\x1B[48;2;123;123;123m. \x1B[0m";
HANDLE hConsole;

int n = 0;

void Render()
{
    DWORD bufferSize1 = 0;
    COORD bufferCoord = {0, 0};

    // WriteConsoleOutputCharacterA(hConsole, (LPCSTR)consoleBuffer, bufferSize, bufferCoord, &bufferSize1);
    WriteConsoleOutputCharacterA(hConsole, (LPCSTR)lenbuf, 1410, bufferCoord, &bufferSize1);
    // WriteConsoleOutputCharacterA(hConsole, (LPCSTR)tmpbuf, sizeof(tmpbuf), bufferCoord, &bufferSize1);
    WriteConsoleOutputCharacterA(hConsole, (LPCSTR)tmp2buf, sizeof(tmp2buf), bufferCoord, &bufferSize1);
    // printf("%s", consoleBuffer);

    // printf("%d\n", bufferSize);
    // printf("%d\n", bufferSize1);
}

int main()
{
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);

    // printf("%d,%d\n", csbi.dwSize.X, csbi.dwSize.Y);

    COORD bufferSize2;
    bufferSize2.X = 1411;
    bufferSize2.Y = 16 * 4;
    // printf("%d\n", SetConsoleScreenBufferSize(hConsole, bufferSize2)); // 设置缓冲区大小
    SetConsoleScreenBufferSize(hConsole, bufferSize2); // 设置缓冲区大小

    SetConsoleActiveScreenBuffer(hConsole);

    GetConsoleScreenBufferInfo(hConsole, &csbi);

    // printf("%d,%d\n", csbi.dwSize.X, csbi.dwSize.Y);


    // printf("%d", sizeof(buf));
    // for (int j = 0; j <= 16; j++)
    // {
    //     for (int i = j * 1410; i < j * 1410 + 32 * (sizeof(buf) - 1); i += (sizeof(buf) - 1))
    //     {
    //         strcpy(&consoleBuffer[i], buf);
    //     }
    //     consoleBuffer[32 * (sizeof(buf) - 1)] = '\r';
    //     consoleBuffer[32 * (sizeof(buf) - 1) + 1] = '\n';
    // }

    // printf("%d", sizeof(lenbuf));
    // for (int i = 0; i < 22561; i+=1410)
    // {
    //     strcpy(&consoleBuffer[i], lenbuf);
    // }
    // consoleBuffer[22561] = '\0';

    bool isRunning = true;

    while (isRunning)
    {
        // gotoxy(0,0)
        COORD pos = {(short)0, (short)0};
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleCursorPosition(hOut, pos);

        // system("cls");
        Render();

        Sleep(16);
    }

    system("pause");

    return 0;
}
