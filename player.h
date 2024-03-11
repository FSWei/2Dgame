#ifndef PLAYER_H
#define PLAYER_H

#include "def.h"
#include "map.h"
#include "camera.h"
#include "roleblock.h"

struct Map;
struct Background;
struct Camera;
struct Roleblock;

struct Player
{
    Map *map;
    Background *background;
    Camera *camera;

    int player_blocksize = 5;
    Roleblock *roleblock;

    pos_ v = {0, 0};         // 速度（其实这个没什么必要，但是作为基础值，还是加上吧）
    bool isAirborne = false; // 角色是否浮空
    bool canJump = true;     // 角色能否跳跃
    bool canFall = false;    // 角色能否下落
    bool isJumping = false;  // 角色是否正在跳跃
    bool isWalking = false;  // 角色是否正在移动
    bool isdash = false;     // 角色是否冲刺
    int xdirection = RIGHT;  // 角色横方向
    int ydirection = DOWN;   // 角色纵方向
    int jump_ticks = 0;       // 跳跃高度
    int updatetime = 100000; // update时间，即移动速度（走一格需要的时间），以后可以通过改变updatetime来实现加速度的模拟

    int dash_cooling_time = DASH_COOLING_TIME; // 突进技能冷却时间

    Player();
    ~Player();
    void update();                              // 更新状态
    void render();                              // 渲染
    bool iscoincide(int y, int x, int object2); // 检查重合
    void move(Roleblock &roleblock);            // 移动
    void dash(Roleblock &roleblock);            // 冲刺
    void skillcool();                           // 技能冷却

    void forEachRoleblock(std::function<void(Roleblock &)> callback);
};

#endif
