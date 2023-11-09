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
    char name = ENEMY;                      // 名字
    char preblock = '.';                    // 上一个位置的信息
    pos_ pos;                               // 坐标
    int direction = 1;                      // 方向，1为右，2为左
    int foregroundcolor = Color(255, 0, 0); // 前景颜色，默认红色
    int backgroundcolor = Color(0, 0, 0);   // 背景颜色，默认黑色

    enemy(int x, int y);

    void update();
    void render();
};

#endif