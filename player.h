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
    pos_ v = {0, 0};           // 速度（其实这个没什么必要，但是作为基础值，还是加上吧）
    bool isAirborne = false;   // 角色是否浮空
    bool isTopReached = false; // 角色是否跳到最高点
    bool isJumping = false;    // 角色是否正在跳跃
    bool isWalking = false;    // 角色是否正在移动
    bool isdash = false;       // 角色是否冲刺
    int xdirection = RIGHT;    // 角色横方向
    int ydirection = DOWN;     // 角色纵方向
    int hop_count = 0;         // 跳跃高度
    int updatetime = 100000;   // update时间，即移动速度（走一格需要的时间），以后可以通过改变updatetime来实现加速度的模拟

    int dash_cooling_time = DASH_COOLING_TIME; // 突进技能冷却时间

    int update();                               // 更新状态
    void render();                              // 渲染
    bool iscoincide(int y, int x, int object2); // 检查重合
    void move();                                // 移动
    void dash();                                // 冲刺
    void skillcool();                           // 技能冷却
};

#endif
