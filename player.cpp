#include "player.h"

Player::Player()
{
    // roleblock = new Roleblock[player_blocksize];
    // roleblock[0] = Roleblock(PLAYER, _255_255_255, _0_255_0, '.', 2, MAP_HEIGHT - 3);
    // roleblock[1] = Roleblock(PLAYER, _255_255_255, _0_255_0, '.', 2, MAP_HEIGHT - 2);
    // roleblock[2] = Roleblock(PLAYER, _255_255_255, _0_255_0, '.', 3, MAP_HEIGHT - 3);
    // roleblock[3] = Roleblock(PLAYER, _255_255_255, _0_255_0, '.', 3, MAP_HEIGHT - 2);

    // roleblock = new Roleblock[1];
    // roleblock[0] = Roleblock(PLAYER, _255_255_255, _0_255_0, 2, MAP_HEIGHT - 2);

    roleblock = new Roleblock[player_blocksize];
    roleblock[0] = Roleblock(PLAYER, _255_255_255, _0_255_0, '.', 2, MAP_HEIGHT - 4);
    roleblock[1] = Roleblock(PLAYER, _255_255_255, _0_255_0, '.', 2, MAP_HEIGHT - 3);
    roleblock[2] = Roleblock(PLAYER, _255_255_255, _0_255_0, '.', 2, MAP_HEIGHT - 2);
    roleblock[3] = Roleblock(PLAYER, _255_255_255, _0_255_0, '.', 1, MAP_HEIGHT - 3);
    roleblock[4] = Roleblock(PLAYER, _255_255_255, _0_255_0, '.', 3, MAP_HEIGHT - 3);
}

Player::~Player()
{
    delete[] roleblock;
}

void Player::update()
{

    forEachRoleblock([this](Roleblock &roleblock)
                     {
                         // 如果下一个位置是flag，判胜
                         if (iscoincide(roleblock.pos.y + v.y * ydirection, roleblock.pos.x + v.x * xdirection, FLAG))
                         {
                             // return 1;//因为这里改成了回调函数，所以不能直接返回了
                             iswin = 1;
                         }

                        //下面这段先注释掉，用于调试👇

                         // // 如果下一个位置是enemy，判负
                         // if (iscoincide(roleblock.pos.y + v.y * ydirection, roleblock.pos.x + v.x * xdirection, ENEMY))
                         // {
                         //     // return 0;
                         //     iswin = 0;
                         // }
                     });

    // 使用回调函数循环遍历所有的 Roleblock 对象
    forEachRoleblock([this](Roleblock &roleblock)
                     { move(roleblock); });

    canJump = true;
    for (int i = 0; i < player_blocksize; ++i)
    {
        if (roleblock[i].canJump == false)
        {
            canJump = false;
            break;
        }
    }

    canFall = true;
    for (int i = 0; i < player_blocksize; ++i)
    {
        if (roleblock[i].canFall == false)
        {
            canFall = false;
            break;
        }
    }
    
    if (canJump && isJumping) // 如果能跳跃并且正在跳跃
    {
        ydirection = UP;
        v.y = 1;
        jump_ticks++;
    }

    else if (canFall) // 如果可以下落
    {
        // 下落的过程中不能再跳跃
        canJump = false;
        ydirection = DOWN;
        v.y = 1;
    }

    forEachRoleblock([this](Roleblock &roleblock)
                     {
        // 👇这个判断是防止角色移动到障碍物的角落时穿模
        //  判断下一个位置是否是自己，如果是自己则跳过，如果不是自己则进行下面的判定
        if (!iscoincide(roleblock.pos.y + v.y * ydirection, roleblock.pos.x + v.x * xdirection, PLAYER))
        {
            // 如果下一个位置有墙，则停止移动
            if (iscoincide(roleblock.pos.y + v.y * ydirection, roleblock.pos.x + v.x * xdirection, WALL))
            {
                v.y = 0;
                v.x = 0;
            }
    } });

    forEachRoleblock([this](Roleblock &roleblock)
                     { dash(roleblock); });

    forEachRoleblock([this](Roleblock &roleblock)
                     {
    // 位置更新

    // 如果角色不是静止并且运动趋势的相反位置上是自己，则把preblock改为自己，这样就不会把自己的位置改成空格或.
    if (v.x != 0 && v.y != 0 && iscoincide(roleblock.pos.y - v.y * ydirection, roleblock.pos.x - v.x * xdirection, PLAYER))
        roleblock.preblock = PLAYER;
    map->blocktype[roleblock.pos.y][roleblock.pos.x].type = roleblock.preblock;

    roleblock.pos.x += v.x * xdirection;
    roleblock.pos.y += v.y * ydirection;
    if (!iscoincide(roleblock.pos.y, roleblock.pos.x, PLAYER) && !iscoincide(roleblock.pos.y, roleblock.pos.x, ENEMY)) // 如果下一个位置不是敌人
    {
        roleblock.preblock = map->blocktype[roleblock.pos.y][roleblock.pos.x].type;
    }
//  if (!iscoincide(roleblock.pos.y, roleblock.pos.x, PLAYER))
    {
        roleblock.backgroundcolor = map->blocktype[roleblock.pos.y][roleblock.pos.x].backgroundcolor;
        map->blocktype[roleblock.pos.y][roleblock.pos.x].type = roleblock.character;
    } });
}

void Player::render()
{
    forEachRoleblock([this](Roleblock &roleblock)
                     {
    SetColor(roleblock.foregroundcolor, roleblock.backgroundcolor);
    camera->renderObject(roleblock.pos.x, roleblock.pos.y, map->blocktype[roleblock.pos.y][roleblock.pos.x].type);
    UnsetColor(roleblock.foregroundcolor, roleblock.backgroundcolor); });
}

bool Player::iscoincide(int y, int x, int object2)
{
    if (map->blocktype[y][x].type == object2)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Player::move(Roleblock &roleblock) // 移动
{
    // X轴判定
    {
        if (isWalking)
            v.x = 1;
        else
            v.x = 0;

        // 判断横向的下一个位置是否是自己，如果是自己则跳过，如果不是自己则进行下面的判定
        if (!iscoincide(roleblock.pos.y, roleblock.pos.x + v.x * xdirection, PLAYER))
        {
            // 如果横向的下一个位置有墙，则停止横向移动
            if (iscoincide(roleblock.pos.y, roleblock.pos.x + v.x * xdirection, WALL))
            {
                v.x = 0;
            }
        }
    }

    // Y轴判定
    {
        // 下方判定
        if (!iscoincide(roleblock.pos.y + 1, roleblock.pos.x, PLAYER)) // 如果下面不是自己，则进行下方判定
        {
            if (iscoincide(roleblock.pos.y + 1, roleblock.pos.x, WALL)) // 如果下面是障碍物，不能下落
            {
                roleblock.canFall = false;
                roleblock.canJump = true;
                jump_ticks = 0;
                v.y = 0;
            }
            else if (!iscoincide(roleblock.pos.y + 1, roleblock.pos.x, WALL)) // 如果下面没有障碍物，即可下落
            {
                roleblock.canFall = true;
                // roleblock.canJump = true;
            }
            else
            {
                roleblock.canJump = true;
            }
        }

        // 上方判定
        else if (!iscoincide(roleblock.pos.y - 1, roleblock.pos.x, PLAYER)) // 如果上面不是自己，则进行上方判定
        {
            if (iscoincide(roleblock.pos.y - 1, roleblock.pos.x, WALL)) // 角色上面碰到墙，不能跳跃
            {
                roleblock.canJump = false;
            }

            else if (jump_ticks == MAXHOP) // 如果跳跃次数达到最大值，则不能再跳跃
            {
                roleblock.canJump = false;
            }

            else
            {
                // canJump2 = true;
                roleblock.canJump = true;
            }
        }
        else
        {
            roleblock.canJump = true;
            roleblock.canFall = true;
        }

        // if (canJump && isJumping) // 如果能跳跃并且正在跳跃
        // {
        //     ydirection = UP;
        //     v.y = 1;
        // }

        // // if (!canJump) // 如果不能跳跃
        // if (!canJump && canFall) // 如果可以下落
        // {
        //     ydirection = DOWN;
        //     v.y = 1;
        // }
    }

    // // 👇这个判断是防止角色移动到障碍物的角落时穿模
    // //  判断下一个位置是否是自己，如果是自己则跳过，如果不是自己则进行下面的判定
    // if (!iscoincide(roleblock.pos.y + v.y * ydirection, roleblock.pos.x + v.x * xdirection, PLAYER))
    // {
    //     // 如果下一个位置有墙，则停止移动
    //     if (iscoincide(roleblock.pos.y + v.y * ydirection, roleblock.pos.x + v.x * xdirection, WALL))
    //     {
    //         v.y = 0;
    //         v.x = 0;
    //     }
    // }
}

void Player::dash(Roleblock &roleblock) // 冲刺突进技能
{
    if (dash_cooling_time == 0)
    {
        roleblock.foregroundcolor = _255_255_255;
    }
    if (isdash == true && dash_cooling_time == 0) // 冲刺突进技能
    {
        dash_cooling_time = DASH_COOLING_TIME;
        roleblock.foregroundcolor = _255_0_255;

        if (canJump == false && isJumping == true) // 如果角色在冲刺时恰好不能跳跃（在最高点）并且按着跳跃键，则把ydirection设置成-1，让它仍能往上冲刺，而不是canJump==false让ydirection=1
        {
            ydirection = UP;
        }

        if (ydirection == DOWN) // 如果纵向方向向下，则v.y=0,不让它向下冲刺
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
            if (iscoincide(roleblock.pos.y + y * ydirection, roleblock.pos.x + x * xdirection, WALL))
            {
                flag = 1;
                tmp -= 2;
                break;
            }
        }
        // 上面的循环里最后一次循环结束后会多出一次没有被检测的值，这个值就是突进的终点位置，所以加上这句，在循环结束后再检测一次，不加的话会出现突进穿墙bug
        if (flag == 0 && iscoincide(roleblock.pos.y + y * ydirection, roleblock.pos.x + x * xdirection, WALL))
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

void Player::skillcool()
{
    if (dash_cooling_time > 0)
    {
        dash_cooling_time--;
    }
}

void Player::forEachRoleblock(std::function<void(Roleblock &)> callback)
{
    for (int i = 0; i < player_blocksize; ++i)
    {
        callback(roleblock[i]);
    }
}
