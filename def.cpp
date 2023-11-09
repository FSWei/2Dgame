#include "def.h"
void gotoxy(int x, int y)
{ // 移动光标
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

int Color(int r, int g, int b) // 打包颜色
{
    return (r << 20) | (g << 10) | b;
}

void SetColor(int foregroundcolor, int backgroundcolor)
{
    short fr = (foregroundcolor >> 20) & 1023; // 获取前 10 位
    short fg = (foregroundcolor >> 10) & 1023; // 获取中间 10 位
    short fb = foregroundcolor & 1023;         // 获取后 10 位
    short br = (backgroundcolor >> 20) & 1023; // 获取前 10 位
    short bg = (backgroundcolor >> 10) & 1023; // 获取中间 10 位
    short bb = backgroundcolor & 1023;         // 获取后 10 位

    printf("\x1B[38;2;%d;%d;%dm\x1B[48;2;%d;%d;%dm", fr, fg, fb, br, bg, bb);
}

extern void UnsetColor()
{
    printf("\x1B[0m");
}

int _time = 0;