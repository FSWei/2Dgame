#ifndef ROLEBLOCK_H
#define ROLEBLOCK_H

#include "def.h"

struct Roleblock
{
    char character;      // 此方块的字符
    int foregroundcolor; // 前景颜色
    int backgroundcolor; // 背景颜色
    char preblock;       // 上一个方块的字符
    pos_ pos;            // 坐标

    short canJump;
    short canFall;

    Roleblock(){};
    Roleblock(char c, int f, int b, char pre, int x, int y) : character(c), foregroundcolor(f), backgroundcolor(b), preblock(pre), pos({x, y}) {}
    bool iscoincide(int y, int x, int object2); // 检查重合
};

#endif