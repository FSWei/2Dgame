#ifndef MAP_H
#define MAP_H

#include "def.h"
#include "player.h"

struct Background;
struct Player;

struct Block
{
    char type;
    int foregroundcolor = _255_255_255;
    int backgroundcolor = _0_0_0;
};

struct Map
{
    Background *background;
    Player *player;
    Block blocktype[MAP_HEIGHT][MAP_LENGTH];

    // map();
    void init();
    void show();
    void camera_show(); // 因为写在camera里的话还要引用map.h，索性直接写在map里
};

struct Background
{
    Block blocktype[MAP_HEIGHT][MAP_LENGTH];
    Background();
    void show();
};

#endif