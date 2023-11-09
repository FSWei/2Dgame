#ifndef MAP_H
#define MAP_H

#include "def.h"
#include "player.h"
#include "enemy.h"

struct background;
struct player;
struct enemy;

struct block
{
    char type;
    int foregroundcolor = Color(255,255,255);
    int backgroundcolor = Color(0, 0, 0);
};

struct map
{
    background *bg;
    player *p;
    enemy *e[MAXENEMY]; // 指针数组
    block blocktype[MAP_HEIGHT][MAP_LENGTH];

    // map();
    void init();
    void show();
    void camera_show(); // 因为写在camera里的话还要引用map.h，索性直接写在map里
};

struct background
{
    block blocktype[MAP_HEIGHT][MAP_LENGTH];
    background();
    void show();
};

#endif