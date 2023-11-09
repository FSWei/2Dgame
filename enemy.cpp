#include "enemy.h"

enemy::enemy(int x, int y)
{
    pos.x = x;
    pos.y = y;
}

void enemy::update()
{
    if (m->blocktype[pos.y][pos.x + direction].type == WALL || m->blocktype[pos.y + 1][pos.x + direction].type != WALL)
    {
        direction *= -1;
    }

    m->blocktype[pos.y][pos.x].type = preblock;
    pos.x += direction;

    if (m->blocktype[pos.y][pos.x].type != ENEMY && m->blocktype[pos.y][pos.x].type != PLAYER)
        preblock = m->blocktype[pos.y][pos.x].type;

    backgroundcolor = m->blocktype[pos.y][pos.x].backgroundcolor;
    m->blocktype[pos.y][pos.x].type = name;
}

void enemy::render()
{
    if (c->isInsideCamera(pos.x, pos.y))
    {
        SetColor(foregroundcolor, backgroundcolor);
        c->renderObject(pos.x, pos.y, m->blocktype[pos.y][pos.x].type);
        UnsetColor();
    }
}