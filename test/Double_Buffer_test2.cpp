#include <iostream>
#include <Windows.h>
#include <string>
#include <unistd.h>
#include <time.h>

const int screenWidth = 90;
const int screenHeight = 64;

// 这两个+1，一个是为了字符，一个是为了字符后面的空格
const int bufferSize = screenWidth * (42 + 1 + 1) * screenHeight + screenWidth; // 后面+的这个screenWidth是为\n服务的

char consoleBuffer[bufferSize];
HANDLE hConsole;

char *Color(int fr, int fg, int fb, int br, int bg, int bb)
{
    // char s[38];
    char *s = new char[38 + 1]; // 注意\0，所以+1
    // 以格式化方式给s赋值
    sprintw(s, "\x1B[38;2;%03d;%03d;%03dm\x1B[48;2;%03d;%03d;%03dm", fr, fg, fb, br, bg, bb);
    return s;
}

void ClearScreen()
{
    for (int i = 0; i < screenWidth * screenHeight; i++)
    {
        consoleBuffer[i] = ' ';
        if (i != 0 && i % screenWidth == 0)
        {
            consoleBuffer[i] = '\n';
        }
    }
    consoleBuffer[screenWidth * screenHeight] = '\0';
}

int n = 0;
void SetChar(int x, int y, char c, int fr, int fg, int fb, int br, int bg, int bb)
{
    if (x == screenWidth)
    {
        consoleBuffer[y * (screenWidth * (42 + 1 + 1)) + x * (42 + 1 + 1) + n] = '\n';
        n++;
    }
    else
    {
        char *s = Color(fr, fg, fb, br, bg, bb);
        strcpy(&consoleBuffer[y * (screenWidth * (42 + 1 + 1)) + x * (42 + +1 + 1) + n], s);
        delete[] s;
        consoleBuffer[y * (screenWidth * (42 + +1 + 1)) + x * (42 + 1 + 1) + 38 + n] = c;
        char tmp[4 + 1 + 1] = " \x1B[0m"; // 注意\0，所以+1，注意前面的空格，所以+1
        strcpy(&consoleBuffer[y * (screenWidth * (42 + 1 + 1)) + x * (42 + 1 + 1) + 38 + 1 + n], tmp);
    }
}

void Render()
{
    // DWORD bufferSize1 = bufferSize;
    // COORD bufferCoord = {0, 0};

    // WriteConsoleOutputCharacterA(hConsole, (LPCSTR)consoleBuffer, bufferSize, bufferCoord, &bufferSize1);

    printw("%s", consoleBuffer);
}

int main()
{
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    SetConsoleActiveScreenBuffer(hConsole);

    // 随机数种子
    srand((unsigned)time(NULL));

    int fr = 0, fg = 0, fb = 0, br = 0, bg = 0, bb = 0;
    bool isRunning = true;
    while (isRunning)
    {
        MOVECURSOR(0,0)
        COORD pos = {(short)0, (short)0};
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleCursorPosition(hOut, pos);

        n = 0;
        fr += rand() % 512;
        fg += rand() % 512;
        fb += rand() % 512;
        br += rand() % 512;
        bg += rand() % 512;
        bb += rand() % 512;
        // ClearScreen();

        for (int y = 0; y < screenHeight; y++)
        {
            for (int x = 0; x <= screenWidth; x++)
            {
                fr *= rand() % 512;
                fr += rand() % 512;
                fr %= 255;
                fg *= rand() % 512;
                fg += rand() % 512;
                fg %= 255;
                fb *= rand() % 512;
                fb += rand() % 512;
                fb %= 255;
                br *= rand() % 512;
                br += rand() % 512;
                br %= 255;
                bg *= rand() % 512;
                bg += rand() % 512;
                bg %= 255;
                bb *= rand() % 512;
                bb += rand() % 512;
                bb %= 255;

                // printw("fr:%d,fg:%d,fb:%d,br:%d,bg:%d,bb:%d     \n", fr, fg, fb, br, bg, bb);
                SetChar(x, y, 32 + (x + y + fr + fg + fb + br + bg + bb) % (127 - 32), fr, fg, fb, br, bg, bb);
            }
        }

        Render();
        // printw("fr:%d,fg:%d,fb:%d,br:%d,bg:%d,bb:%d     \n", fr, fg, fb, br, bg, bb);

        Sleep(16);
    }

    system("pause");

    return 0;
}
