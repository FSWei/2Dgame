#ifndef PLAYER_H
#define PLAYER_H

#include "def.h"
#include "map.h"
#include "camera.h"
#include "enemy.h"

struct map;
struct background;
struct enemy;
struct camera;

struct player
{
    map *m;
    background *bg;
    camera *c;
    enemy *e[MAXENEMY];

    char name = PLAYER;
    int foregroundcolor = _255_255_255; // 前景颜色，默认为白色
    int backgroundcolor = _0_255_0;     // 背景颜色，默认为绿色
    char preblock = BG;
    pos_ pos = {2, MAP_HEIGHT - 2};
    pos_ v = {0, 0};                    // 速度
    bool isair = false;                 // 角色是否浮空
    bool istop = false;                 // 角色是否跳到最高点
    bool isjump = false;                // 角色是否正在跳跃
    bool iswalk = false;                // 角色是否正在移动
    bool isdash = false;                // 角色是否冲刺
    int xdirection = RIGHT;             // 角色横方向
    int ydirection = DOWN;              // 角色纵方向
    int hop_count;                      // 跳跃高度
    int updatetime = PLAYER_UPDATETIME; // update时间,以后可以通过改变updatetime来实现加速度的模拟

    int dash_cooling_time = 0; // 突进技能冷却时间，单位为PLAYER_UPDATETIME

    void input();         // 检测输入
    int update();         // 更新状态
    void render();        // 渲染
    void camera_render(); // 渲染摄像机，暂时放在player里
    void timer();         // 计时器，应该放在game类里，暂时放在这里
    void dash();
};

#endif
