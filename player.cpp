#include "player.h"

int player::update()
{
    move();
    dash();
    // 如果下一个位置是flag，判胜
    if (iscoincide(pos.y + v.y * ydirection, pos.x + v.x * xdirection, FLAG))
    {
        return 1;
    }
    // 如果下一个位置是enemy，判负
    if (iscoincide(pos.y + v.y * ydirection, pos.x + v.x * xdirection, ENEMY))
    {
        return 0;
    }

    // 位置更新
    m->blocktype[pos.y][pos.x].type = preblock;

    pos.x += v.x * xdirection;
    pos.y += v.y * ydirection;
    if (m->blocktype[pos.y][pos.x].type != ENEMY)
        preblock = m->blocktype[pos.y][pos.x].type;

    backgroundcolor = m->blocktype[pos.y][pos.x].backgroundcolor;
    m->blocktype[pos.y][pos.x].type = name;

    return -1;
}

void player::render()
{
    SetColor(foregroundcolor, backgroundcolor);
    c->renderObject(pos.x, pos.y, m->blocktype[pos.y][pos.x].type);
    UnsetColor(foregroundcolor, backgroundcolor);
}

bool player::iscoincide(int y, int x, int object2)
{
    if (m->blocktype[y][x].type == object2)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void player::move() // 移动
{
    if (isWalking == true)
        v.x = 1;
    else
        v.x = 0;

    if (iscoincide(pos.y + 1, pos.x, WALL)) // 如果下面是障碍物，代表角色站在地上，既不浮空也没到顶，跳跃次数为0
    {
        isAirborne = false;
        v.y = 0;
        isTopReached = false;
        hop_count = 0;
    }
    else // 如果没有障碍物，表示浮空
    {
        isAirborne = true;
        v.y = 1;
    }
    if (isTopReached || iscoincide(pos.y - 1, pos.x, WALL)) // 如果角色跳跃到最高点或者角色上面碰到墙，直接下落
    {
        isTopReached = true;
        ydirection = DOWN;
        hop_count--;
    }

#ifdef _WIN32
    else if (isAirborne && !(GetAsyncKeyState(0x57) & 0x8000 || GetAsyncKeyState(0x26) & 0x8000)) // 如果角色浮空并且没有按W或上，则一直下降
#elif __linux__
    else if (isAirborne && !(key_state[17] || key_state[103]))
#endif
    {
        isTopReached = true;
        ydirection = DOWN;
        hop_count--;
    }
    if (!(iscoincide(pos.y - 1, pos.x, WALL)) && !isTopReached && isJumping == true) // 如果角色上面没有障碍物并且角色没有到达最高点并且按了W或上
    {
        isAirborne = true;
        v.y = 1;
        hop_count++;
        ydirection = UP;
        if (hop_count == MAXHOP)
        {
            isTopReached = true;
        }
    }

    // 如果下一个位置有墙，则停止纵向移动
    if (iscoincide(pos.y + v.y * ydirection, pos.x + v.x * xdirection, WALL))
    {
        v.x = 0;
    }
}

void player::dash() // 冲刺突进技能
{
    if (dash_cooling_time == 0)
    {
        foregroundcolor = _255_255_255;
    }
    if (isdash == true && dash_cooling_time == 0) // 冲刺突进技能
    {
        dash_cooling_time = DASH_COOLING_TIME;
        foregroundcolor = _255_0_255;

        if (isTopReached == true && isJumping == true) // 如果角色在冲刺时恰好在最高点并且按着跳跃键，则把ydirection设置成-1，让它仍能往上冲刺，而不是isTopReached==true让ydirection=1
        {
            ydirection = -1;
        }

        if (ydirection == 1) // 如果纵向方向向下，则v.y=0,不让它向下冲刺
        {
            v.y = 0;
        }
        // 如果路径上有任意一个位置存在WALL，则强制停止冲刺，让角色出现在路径上的WALL之前的一个位置
        int tmp = 1;
        int flag = 0;
        int x = v.x;
        int y = v.y;

        for (; x < DASH_DISTANCE && y < DASH_DISTANCE; x = v.x * tmp, y = v.y * tmp, ++tmp)
        // for (; tmp < DASH_DISTANCE; x = v.x * tmp, y = v.y * tmp, ++tmp)
        {
            if (x == 0 && y == 0)
            {
                tmp = 0;
                break;
            }
            if (iscoincide(pos.y + y * ydirection, pos.x + x * xdirection, WALL))
            {
                flag = 1;
                tmp -= 2;
                break;
            }
        }
        // 上面的循环里最后一次循环结束后会多出一次没有被检测的值，这个值就是突进的终点位置，所以加上这句，在循环结束后再检测一次，不加的话会出现突进穿墙bug
        if (flag == 0 && iscoincide(pos.y + y * ydirection, pos.x + x * xdirection, WALL))
        {
            tmp -= 2;
        }
        // 如果路径上没有出现WALL，则要-1，因为最后一次循环后tmp又++了一次；反之，如果进入了上面两个if中的任意一个，就不会再进入此if，因为根本没有执行到for最后的++，或者已经把多出来的tmp减去了
        if (tmp == DASH_DISTANCE + 1)
        {
            tmp--;
        }

        if (tmp < 0)
        {
            tmp = 0;
        }

        v.x *= tmp;
        v.y *= tmp;

        isdash = false;
    }
}

void player::dashcool()
{
    if (dash_cooling_time > 0)
    {
        dash_cooling_time--;
    }
}