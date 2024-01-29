#ifndef OBJECT_H
#define OBJECT_H

#include "def.h"
#include "map.h"
#include "camera.h"

struct map;
struct background;
struct camera;

class object
{
protected:
    map *m;
    background *bg;
    camera *c;
    char name;
    char preblock;
    pos_ pos;
    int foregroundcolor;
    int backgroundcolor;

public:
    object(map *mapPtr, background *bgPtr, camera *camPtr, char objName, char prevBlock, int x, int y, int fgColor, int bgColor)
        : m(mapPtr), bg(bgPtr), c(camPtr), name(objName), preblock(prevBlock), pos({x, y}), foregroundcolor(fgColor), backgroundcolor(bgColor) {}

    virtual ~object() {}

    virtual void update() = 0;
    virtual void render() = 0;
};

#endif
