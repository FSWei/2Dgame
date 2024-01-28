#ifndef DEF_H
#define DEF_H

#include <stdio.h>
#include <time.h>
#include <vector>
#include <tuple>
#include <thread>
#include <unistd.h>
#include <mutex>

#ifdef _WIN32

#include <Windows.h>
#define MOVECURSOR(x,y) gotoxy(x,y)
#define PRINT printf
#define CLEARWINDOW system("cls")

#elif __linux__

#include <ncursesw/ncurses.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/select.h>
#include <linux/input.h>

#define MOVECURSOR(x,y) move(y,x)
#define PRINT printw
#define CLEARWINDOW clear()

// 不确定具体要监听哪个键盘，所以都写上，然后后面去比较，哪个大就用哪个
#define KEYBOARD_DEVICE_1 "/dev/input/event0"
#define KEYBOARD_DEVICE_2 "/dev/input/event1"

#define MAX_KEYCODE 255 // 最大键码值

extern int key_state[MAX_KEYCODE + 1]; // 全局数组，记录键盘按键状态，初始值为0

#endif

// 代表各种元素的字符
#define PLAYER 'P'
#define WALL 'W'
#define FLAG 'F'
#define ENEMY 'E'
#define BG '.'

// 地图和摄像机的长宽
#define MAP_LENGTH 64
#define MAP_HEIGHT 32
#define CAMERA_LENGTH 32
#define CAMERA_HEIGHT 16

#define LEFT_BORDER 2             // 左侧边界
#define RIGHT_BORDER 2            // 右侧边界
#define TOP_BORDER 2              // 顶端边界
#define BOTTOM_BORDER 2           // 底端边界
#define BORDER_COLOR _255_255_255 // 边框颜色

#define MAXHOP 4                 // 最大跳跃高度
#define MAXENEMY 4               // 敌人数量
#define PLAYER_UPDATETIME 100000 // player更新间隔时间
#define DASH_DISTANCE 5          // 突进距离
#define DASH_COOLING_TIME 5      // 突进冷却时间

enum
{
    LEFT = -1,
    RIGHT = 1,
    UP = -1,
    DOWN = 1
};

// enum // pairnum
// {
//     F_255_255_255__B_255_255_255 = 1, // all white
//     F_0_0_0__B_0_0_0_,                // all black
//     F_255_255_255__B_0_0_0,           // fore white, back black
//     F_255_0_0__B_255_0_0,             // all red
//     F_0_100_20__B_0_100_20,           // all green-blue
//     F_0_0_255__B_0_0_255,             // all blue
//     F_255_0_255__B_255_0_255,         // all purple
//     F_255_255_0__B_255_255_0          // all yellow
// };

enum // colornum
{
    _255_255_255 = 1, // white
    _0_0_0,           // black
    _255_0_0,         // red
    _0_100_20,        // green-blue
    _0_0_255,         // blue
    _255_0_255,       // purple
    _255_255_0,       // yellow
    _0_255_0          // green
};

struct pos_
{
    int x;
    int y;
};

#ifdef _WIN32
extern void gotoxy(int x, int y);
extern void HideConsoleCursor();
extern void ShowConsoleCursor();
#endif
extern int Color(int r, int g, int b);          // 打包颜色
extern void InitColor(int color, int colornum); // 初始化颜色
extern void InitAllcolor();                     // 初始化所有颜色
extern void SetColor(int foregroundcolor, int backgroundcolor);
extern void UnsetColor(int foregroundcolor, int backgroundcolor);
extern int _time; // 游戏时间

#endif