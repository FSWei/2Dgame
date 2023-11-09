#ifndef DEF_H
#define DEF_H

#include <stdio.h>
#include <time.h>
#include <Windows.h>
#include <vector>
#include <tuple>
#include <thread>
#include <unistd.h>
#include <mutex>

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

#define LEFT_BORDER 2                                           // 左侧边界
#define RIGHT_BORDER 2                                          // 右侧边界
#define TOP_BORDER 2                                            // 顶端边界
#define BOTTOM_BORDER 2                                         // 底端边界
#define BORDER_COLOR 255,255,255 // 边框颜色

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

struct pos_
{
    int x;
    int y;
};

extern void gotoxy(int x, int y);
extern void HideConsoleCursor();
extern void ShowConsoleCursor();
extern int Color(int r, int g, int b); // 打包颜色
extern void SetColor(int foregroundcolor, int backgroundcolor);
extern void UnsetColor();
extern int _time; // 游戏时间

#endif