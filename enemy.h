#ifndef ENEMY_H
#define ENEMY_H

#include "map.h"
#include "camera.h"

struct map;
struct background;
struct camera;

struct enemy
{
    map *m;
    background *bg;
    camera *c;

    char name = ENEMY;              // 名字
    char preblock = '.';            // 上一个位置的信息
    pos_ pos;                       // 坐标
    int direction = RIGHT;          // 方向
    int foregroundcolor = _255_0_0; // 前景颜色，默认红色
    int backgroundcolor = _0_0_0;   // 背景颜色，默认黑色
    int updatetime = 200000;        // 更新时间，即移动速度（走一格需要的时间）

    enemy(int x, int y);

    void update();
    void render();
};

#endif