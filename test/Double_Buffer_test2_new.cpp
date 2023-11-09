#include <iostream>
#include <Windows.h>
#include <string>
#include <unistd.h>
#include <time.h>

const int screenWidth = 32;
const int screenHeight = 16;

// 这两个+1，一个是为了字符，一个是为了字符后面的空格
const DWORD bufferSize = screenWidth * (42 + 1 + 1) * screenHeight + screenHeight * 2 + 1; // 后面+的这个screenHeight * 2是为\r\n服务的，最后的+1是为了\0

char consoleBuffer[bufferSize];
HANDLE hConsole;

char *Color(int fr, int fg, int fb, int br, int bg, int bb)
{
    // char s[38];
    char *s = new char[38 + 1]; // 注意\0，所以+1（无需担心\0，后面会自动覆盖掉）
    // 以格式化方式给s赋值
    sprintf(s, "\x1B[38;2;%03d;%03d;%03dm\x1B[48;2;%03d;%03d;%03dm", fr, fg, fb, br, bg, bb);
    return s;
}

// void ClearScreen()
// {
//     for (int i = 0; i < screenWidth * screenHeight; i++)
//     {
//         consoleBuffer[i] = ' ';
//         if (i != 0 && i % screenWidth == 0)
//         {
//             consoleBuffer[i] = '\n';
//         }
//     }
//     consoleBuffer[screenWidth * screenHeight] = '\0';
// }

int n = 0;
void SetChar(int x, int y, char c, int fr, int fg, int fb, int br, int bg, int bb)
{
    if (x == screenWidth)
    {
        // 注意这里的换行要用\r\n，因为在控制台中，换行符\n表示换行，而回车符\r表示回车。当在控制台输出时，遇到换行符\n会导致光标移到下一行的开头位置，但不会将光标移到行首。而回车符\r会将光标移到行首。
        consoleBuffer[y * (screenWidth * (42 + 1 + 1)) + x * (42 + 1 + 1) + n * 2] = '\r';
        consoleBuffer[y * (screenWidth * (42 + 1 + 1)) + x * (42 + 1 + 1) + n * 2 + 1] = '\n';
        // FILE *file = fopen("test.txt", "a");
        // fprintf(file, "\r\n");
        // fclose(file);
        n++;
    }
    else
    {
        char *s = Color(fr, fg, fb, br, bg, bb);
        strcpy(&consoleBuffer[y * (screenWidth * (42 + 1 + 1)) + x * (42 + 1 + 1) + n * 2], s); // 这里的+1+1一个是为了字符，一个是为了空格
        // FILE *file = fopen("test.txt", "a");
        // fprintf(file, "%s", s);
        delete[] s;
        consoleBuffer[y * (screenWidth * (42 + 1 + 1)) + x * (42 + 1 + 1) + 38 + n * 2] = c;
        // fprintf(file, "%c", c);
        char tmp[4 + 1 + 1] = " \x1B[0m"; // 注意\0，所以+1，注意前面的空格，所以+1（无需担心\0，后面会自动覆盖掉）
        // fprintf(file, "%s", tmp);
        // fclose(file);
        strcpy(&consoleBuffer[y * (screenWidth * (42 + 1 + 1)) + x * (42 + 1 + 1) + 38 + 1 + n * 2], tmp);
    }
}

void Render()
{
    DWORD bufferSize1;
    COORD bufferCoord = {0, 0};

    WriteConsoleOutputCharacterA(hConsole, (LPCSTR)consoleBuffer, bufferSize, bufferCoord, &bufferSize1);

    // printf("%d\n", bufferSize);
    // printf("%d\n", bufferSize1);
    
    // FILE *file = fopen("test.txt", "a");
    // fprintf(file, "%d\n%d\n", bufferSize, bufferSize1);
    // fclose(file);

    // printf("%s", consoleBuffer);
    // FILE* file = fopen("test.txt", "w");
    // fprintf(file, "%s", consoleBuffer);
}

int main()
{
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    GetConsoleScreenBufferInfo(hConsole, &csbi);     // 获取缓冲区信息
    printf("%d,%d\n", csbi.dwSize.X, csbi.dwSize.Y); // 打印一下目前缓冲区大小

    COORD bufferSize2;
    bufferSize2.X = screenWidth * (42 + 1 + 1) + 2;
    bufferSize2.Y = screenHeight * 10; // 太小的话会设置失败
    // printf("%d\n", SetConsoleScreenBufferSize(hConsole, bufferSize2)); // 设置缓冲区大小，结果为1则成功
    SetConsoleScreenBufferSize(hConsole, bufferSize2);

    SetConsoleActiveScreenBuffer(hConsole); // 设置活动缓冲区

    GetConsoleScreenBufferInfo(hConsole, &csbi);     // 获取屏幕缓冲区信息
    printf("%d,%d\n", csbi.dwSize.X, csbi.dwSize.Y); // 打印新的缓冲区大小

    // 随机数种子
    srand((unsigned)time(NULL));

    int fr = 0, fg = 0, fb = 0, br = 0, bg = 0, bb = 0;
    bool isRunning = true;

    while (isRunning)
    {
        // gotoxy(0,0)
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

                // printf("fr:%d,fg:%d,fb:%d,br:%d,bg:%d,bb:%d     \n", fr, fg, fb, br, bg, bb);
                // SetChar(x, y, 32 + (x + y + fr + fg + fb + br + bg + bb) % (127 - 32), fr, fg, fb, br, bg, bb);
                SetChar(x, y, '.', 123, 123, 123, 123, 123, 123);
            }
        }

        consoleBuffer[bufferSize] = '\0';
        Render();
        // printf("fr:%d,fg:%d,fb:%d,br:%d,bg:%d,bb:%d     \n", fr, fg, fb, br, bg, bb);

        Sleep(16);
    }

    system("pause");

    return 0;
}
