#include "player.h"

void player::input()
{
    // if ((GetAsyncKeyState(0x57) & 0x8000 || GetAsyncKeyState(0x26) & 0x8000)) // w、↑
    // {
    //     isjump = true;
    // }

    // if ((GetAsyncKeyState(0x44) & 0x8000 || GetAsyncKeyState(0x27) & 0x8000)) // d、→
    // {
    //     iswalk = true;
    //     xdirection = RIGHT;
    // }

    // else if ((GetAsyncKeyState(0x41) & 0x8000 || GetAsyncKeyState(0x25) & 0x8000)) // a、←
    // {
    //     iswalk = true;
    //     xdirection = LEFT;
    // }

    // if (GetAsyncKeyState(0x4A) & 0x8000) // j
    // {
    //     isdash = true;
    // }
    

    // ch = getch(); // 获取当前按键

    // if (ch == 'w' || ch == KEY_UP) // 对应 W 键或向上箭头键
    // {

    //     isjump = true;
    // }
    // if (ch == 'd' || ch == KEY_RIGHT) // 对应 D 键或向右箭头键
    // {
    //     iswalk = true;
    //     xdirection = RIGHT;
    // }
    // else if (ch == 'a' || ch == KEY_LEFT) // 对应 A 键或向左箭头键
    // {
    //     iswalk = true;
    //     xdirection = LEFT;
    // }
    // if (ch == 'j') // 对应 J 键
    // {
    //     isdash = true;
    // }

    if (key_state[17]||key_state[103]) // 对应 W 键或向上箭头键
    {
        isjump = true;
    }
    if (key_state[32]||key_state[106]) // 对应 D 键或向右箭头键
    {
        iswalk = true;
        xdirection = RIGHT;
    }
    else if (key_state[30]||key_state[105]) // 对应 A 键或向左箭头键
    {
        iswalk = true;
        xdirection = LEFT;
    }
    if (key_state[36]) // 对应 J 键
    {
        isdash = true;
    }
}

int player::update()
{
    if (iswalk == true)
        v.x = 1;
    else
        v.x = 0;

    if (m->blocktype[pos.y + 1][pos.x].type == WALL) // 如果下面是障碍物，代表角色站在地上，既不浮空也没到顶，跳跃次数为0
    {
        isair = false;
        v.y = 0;
        istop = false;
        hop_count = 0;
    }
    else // 如果没有障碍物，表示浮空
    {
        isair = true;
        v.y = 1;
    }
    if (istop || m->blocktype[pos.y - 1][pos.x].type == WALL) // 如果角色跳跃到最高点或者角色上面碰到墙，直接下落
    {
        istop = true;
        ydirection = DOWN;
        hop_count--;
    }
    // else if (isair && !(GetAsyncKeyState(0x57) & 0x8000 || GetAsyncKeyState(0x26) & 0x8000)) // 如果角色浮空并且没有按W或上，则一直下降
    else if (isair && !(key_state[17]||key_state[103]))
    {
        istop = true;
        ydirection = DOWN;
        hop_count--;
    }
    if (m->blocktype[pos.y - 1][pos.x].type != WALL && !istop && isjump == true) // 如果角色上面没有障碍物并且角色没有到达最高点并且按了W或上
    {
        isair = true;
        v.y = 1;
        hop_count++;
        ydirection = UP;
        if (hop_count == MAXHOP)
        {
            istop = true;
        }
    }

    dash();

    // 如果左键右键a键d键都松开，iswalk = false
    // if (!(GetAsyncKeyState(0x44) & 0x8000 || GetAsyncKeyState(0x27) & 0x8000 || GetAsyncKeyState(0x41) & 0x8000 || GetAsyncKeyState(0x25) & 0x8000))
    if (!(key_state[30]||key_state[105]||key_state[32]||key_state[106]))
    {
        iswalk = false;
    }
    // 如果上键松开，isjump = false
    // if (!(GetAsyncKeyState(0x57) & 0x8000 || GetAsyncKeyState(0x26) & 0x8000))
    if (!(key_state[17]||key_state[103]))
    {
        isjump = false;
    }
    // 如果j键松开，jsdash=false
    // if (!(GetAsyncKeyState(0x4A) & 0x8000))
    if (!(key_state[36]))
    {
        isdash = false;
    }

    // 如果下一个位置有墙，则停止纵向移动
    if (m->blocktype[pos.y + v.y * ydirection][pos.x + v.x * xdirection].type == WALL)
    {
        v.x = 0;
    }
    // 如果下一个位置是flag，判胜
    if (m->blocktype[pos.y + v.y * ydirection][pos.x + v.x * xdirection].type == FLAG)
    {
        return 1;
    }
    // 如果下一个位置是enemy，判负
    if (m->blocktype[pos.y + v.y * ydirection][pos.x + v.x * xdirection].type == ENEMY)
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

// int tmp = 1;
// 这个全图渲染暂时放在这里
void player::camera_render()
{
    for (int y = c->top; y < c->top + c->height; y++)
    {
        for (int x = c->left; x < c->left + c->width; x++)
        {
            if (c->isInsideCamera(x, y)) // 检查坐标是否在C范围内
            {
                // gotoxy((LEFT_BORDER + (x - c->left)) * 2, TOP_BORDER + (y - c->top));
                if (m->blocktype[y][x].type == PLAYER)
                {
                    // SetColor(foregroundcolor , backgroundcolor);
                    // printw("%c ", m->blocktype[y][x].type);
                    // UnsetColor();
                }
                else if (m->blocktype[y][x].type == ENEMY)
                {
                    // SetColor(e[0]->foregroundcolor | e[0]->backgroundcolor);
                    // UnsetColor();
                }
                else
                {
                    SetColor(m->blocktype[y][x].foregroundcolor, m->blocktype[y][x].backgroundcolor);
                    // printw("%c ", m->blocktype[y][x].type);
                    c->renderObject(x, y, m->blocktype[y][x].type);
                    UnsetColor(m->blocktype[y][x].foregroundcolor, m->blocktype[y][x].backgroundcolor);
                }
                // printw("%c ", m->blocktype[y][x].type);//不可以写在这里，因为颜色
            }
        }
    }
}

void player::timer()
{
    _time++;
    if (dash_cooling_time > 0)
    {
        dash_cooling_time--;
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

        if (istop == true && isjump == true) // 如果角色在冲刺时恰好在最高点并且按着跳跃键，则把ydirection设置成-1，让它仍能往上冲刺，而不是istop==true让ydirection=1
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
            if (m->blocktype[pos.y + y * ydirection][pos.x + x * xdirection].type == WALL)
            {
                flag = 1;
                tmp -= 2;
                break;
            }
        }
        // 上面的循环里最后一次循环结束后会多出一次没有被检测的值，这个值就是突进的终点位置，所以加上这句，在循环结束后再检测一次，不加的话会出现突进穿墙bug
        if (flag == 0 && m->blocktype[pos.y + y * ydirection][pos.x + x * xdirection].type == WALL)
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