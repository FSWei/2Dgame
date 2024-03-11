#include "enemy.h"

Enemy::Enemy(int x, int y)
{
    pos.x = x;
    pos.y = y;
}

void Enemy::update()
{
    if (map->blocktype[pos.y][pos.x + direction].type == WALL || map->blocktype[pos.y + 1][pos.x + direction].type != WALL)
    {
        direction *= -1;
    }

    map->blocktype[pos.y][pos.x].type = preblock;
    pos.x += direction;

    if (map->blocktype[pos.y][pos.x].type != ENEMY && map->blocktype[pos.y][pos.x].type != PLAYER) // 如果下一个位置不是enemy和player
        preblock = map->blocktype[pos.y][pos.x].type;                                              // 记录下一个位置的方块类型

    backgroundcolor = map->blocktype[pos.y][pos.x].backgroundcolor;
    map->blocktype[pos.y][pos.x].type = name;
}

void Enemy::render()
{
    if (camera->isInsideCamera(pos.x, pos.y))
    {
        SetColor(foregroundcolor, backgroundcolor);
        camera->renderObject(pos.x, pos.y, map->blocktype[pos.y][pos.x].type);
        UnsetColor(foregroundcolor, backgroundcolor);
    }
}