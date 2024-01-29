#include "def.h"

#ifdef _WIN32
void gotoxy(int x, int y)
{
    // 移动光标
    COORD pos = {(short)x, (short)y};
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hOut, pos);
}

// 隐藏光标
void HideConsoleCursor()
{
    CONSOLE_CURSOR_INFO cursor_info = {1, 0};
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorInfo(hOut, &cursor_info);
}

void ShowConsoleCursor()
{
    CONSOLE_CURSOR_INFO cursor_info = {1, 1}; // 将 dwSize 设置为非零值以显示光标
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorInfo(hOut, &cursor_info);
}

int ColorArr[255];

#endif

int Color(int r, int g, int b) // 打包颜色
{
    return (r << 20) | (g << 10) | b;
}

// void SetColor(int foregroundcolor, int backgroundcolor)
// {
//     short fr = (foregroundcolor >> 20) & 1023; // 获取前 10 位
//     short fg = (foregroundcolor >> 10) & 1023; // 获取中间 10 位
//     short fb = foregroundcolor & 1023;         // 获取后 10 位
//     short br = (backgroundcolor >> 20) & 1023; // 获取前 10 位
//     short bg = (backgroundcolor >> 10) & 1023; // 获取中间 10 位
//     short bb = backgroundcolor & 1023;         // 获取后 10 位

//     PRINT("\x1B[38;2;%d;%d;%dm\x1B[48;2;%d;%d;%dm", fr, fg, fb, br, bg, bb);
// }

// extern void UnsetColor()
// {
//     PRINT("\x1B[0m");
// }

// 在windows中用数组存储颜色，在linux中用init_color初始化颜色
void InitColor(int colornum, int color)
{
#ifdef _WIN32
    ColorArr[colornum] = color;
#elif __linux__
    // 定义颜色
    init_color(colornum, ((color >> 20) & 1023) * 1000 / 255, ((color >> 10) & 1023) * 1000 / 255, (color & 1023) * 1000 / 255);
#endif
}

void InitAllcolor()
{
    InitColor(_0_0_0, Color(0, 0, 0));
    InitColor(_255_255_255, Color(255, 255, 255));
    InitColor(_0_100_20, Color(0, 100, 20));
    InitColor(_0_0_255, Color(0, 0, 255));
    InitColor(_255_0_0, Color(255, 0, 0));
    InitColor(_255_0_255, Color(255, 0, 255));
    InitColor(_255_255_0, Color(255, 255, 0));
    InitColor(_0_255_0, Color(0, 255, 0));
}

// Only applicable when the total number of colors is less than 10,
// because the first parameter of init_color and init_pair only supports the range 0-255
void SetColor(int foregroundcolor, int backgroundcolor)
{
#ifdef _WIN32
    int f = ColorArr[foregroundcolor];
    int b = ColorArr[backgroundcolor];
    short fr = (f >> 20) & 1023; // 获取前 10 位
    short fg = (f >> 10) & 1023; // 获取中间 10 位
    short fb = f & 1023;         // 获取后 10 位
    short br = (b >> 20) & 1023; // 获取前 10 位
    short bg = (b >> 10) & 1023; // 获取中间 10 位
    short bb = b & 1023;         // 获取后 10 位

    PRINT("\x1B[38;2;%d;%d;%dm\x1B[48;2;%d;%d;%dm", fr, fg, fb, br, bg, bb);
#elif __linux__
    // 定义颜色对。（foregroundcolor << 4) | backgroundcolor可以当作键来对待，它对应前景和背景两个值）
    init_pair((foregroundcolor << 4) | backgroundcolor, foregroundcolor, backgroundcolor);
    // 应用颜色对
    attron(COLOR_PAIR((foregroundcolor << 4) | backgroundcolor));
#endif
}

void UnsetColor(int foregroundcolor, int backgroundcolor)
{
#ifdef _WIN32
    PRINT("\x1B[0m");
#elif __linux__
    // 关闭颜色对
    attroff(COLOR_PAIR((foregroundcolor << 4) | backgroundcolor));
#endif
}

int _time = 0;
int iswin = -1;
int gamestatus = 1; // 0是结束，1是正常状态
std::mutex mtx;     // 使用互斥锁，避免因为两者同时渲染而出现的光标混乱问题

#ifdef __linux__
int key_state[MAX_KEYCODE + 1] = {0};
#endif