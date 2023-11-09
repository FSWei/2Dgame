#include "map.h"

// map::map()
// {
// }

void map::init()
{
    for (int i = 0; i < MAP_HEIGHT; i++)
    {
        for (int j = 0; j < MAP_LENGTH; j++)
        {
            blocktype[i][j].type = bg->blocktype[i][j].type; // 直接把背景赋到map上
            blocktype[i][j].foregroundcolor = bg->blocktype[i][j].foregroundcolor;
            blocktype[i][j].backgroundcolor = bg->blocktype[i][j].backgroundcolor;

            blocktype[i][0].type = WALL;
            blocktype[i][MAP_LENGTH - 1].type = WALL;

            blocktype[0][j].type = WALL;
            blocktype[MAP_HEIGHT - 1][j].type = WALL;
        }
    }
    for (int i = 0; i < 10; i++)
    {
        blocktype[28][i + 5].type = WALL;
        blocktype[25][i + 10].type = WALL;
        blocktype[22][i + 15].type = WALL;
        blocktype[19][i + 20].type = WALL;
        blocktype[25][i + 40].type = WALL;
    }
    // blocktype[p->pos.y][p->pos.x].type = p->name; // 人物
    // blocktype[e[0]->pos.y][e[0]->pos.x].type = e[0]->name;
    // blocktype[e[1]->pos.y][e[1]->pos.x].type = e[1]->name;
    // blocktype[e[2]->pos.y][e[2]->pos.x].type = e[2]->name;
    blocktype[24][48].type = FLAG; // 旗帜

    // 上色
    for (int i = 0; i < MAP_HEIGHT; i++)
    {
        for (int j = 0; j < MAP_LENGTH; j++)
        {
            if (blocktype[i][j].type == WALL)
            {
                blocktype[i][j].foregroundcolor = Color(0, 0, 255);
                blocktype[i][j].backgroundcolor = Color(0, 0, 255);
            }
            else if (blocktype[i][j].type == FLAG)
            {
                blocktype[i][j].foregroundcolor = Color(255, 255, 0);
            }
        }
    }
}

void map::show()
{
    for (int i = 0; i < MAP_HEIGHT; i++)
    {
        for (int j = 0; j < MAP_LENGTH; j++)
        {
            if (blocktype[i][j].type == WALL)
            {
                SetColor(blocktype[i][j].foregroundcolor, blocktype[i][j].backgroundcolor);
                printf("%c ", blocktype[i][j].type);
                UnsetColor();
            }
            else if (blocktype[i][j].type == BG)
            {
                SetColor(blocktype[i][j].foregroundcolor, blocktype[i][j].backgroundcolor);
                printf("%c ", blocktype[i][j].type);
                UnsetColor();
            }
            else
            {
                printf("%c ", blocktype[i][j].type);
            }
        }
        printf("\n");
    }
}

void map::camera_show()
{
    for (int y = p->c->top; y < p->c->top + p->c->height; y++)
    {
        for (int x = p->c->left; x < p->c->left + p->c->width; x++)
        {
            if (p->c->isInsideCamera(x, y)) // 检查坐标是否在C范围内
            {
                // gotoxy((x - p->c->left) * 2, y - p->c->top);
                if (blocktype[y][x].type == WALL)
                {
                    SetColor(blocktype[y][x].foregroundcolor, blocktype[y][x].backgroundcolor);
                }
                else if (blocktype[y][x].type == BG)
                {
                    SetColor(blocktype[y][x].foregroundcolor, blocktype[y][x].backgroundcolor);
                }
                else
                {
                    SetColor(Color(255, 255, 255), Color(0, 0, 0));
                }
                printf("%c ", blocktype[y][x].type);
                UnsetColor();
            }
        }
        printf("\n");
    }
}

background::background()
{
    char tmp[MAP_HEIGHT][MAP_LENGTH] = {"                                                               ",
                                        "                                                               ",
                                        "                                                               ",
                                        "                                                               ",
                                        "                                                               ",
                                        "                                                               ",
                                        "                                                               ",
                                        "                                                               ",
                                        "                                                               ",
                                        "                                                               ",
                                        "                                                               ",
                                        "              ...                                              ",
                                        "             .....              .....                          ",
                                        "            .......            .......                         ",
                                        "           .........          .........         ......         ",
                                        ".         ..........         ..........        ........        ",
                                        "..       ............      .............      ..........      .",
                                        "..      .............     ...............    ............    ..",
                                        "...     ..............   .................   .............  ...",
                                        "...    ...............  ................... ...................",
                                        "....  .........................................................",
                                        "...............................................................",
                                        "...............................................................",
                                        "...............................................................",
                                        "...............................................................",
                                        "...............................................................",
                                        "...............................................................",
                                        "...............................................................",
                                        "...............................................................",
                                        "...............................................................",
                                        "...............................................................",
                                        "..............................................................."};

    for (int i = 0; i < MAP_HEIGHT; i++)
    {
        {
            for (int j = 0; j < MAP_LENGTH; j++)
            {
                blocktype[i][j].type = tmp[i][j];
                if (blocktype[i][j].type == '.')
                {
                    blocktype[i][j].foregroundcolor = Color(0, 100, 20);
                    blocktype[i][j].backgroundcolor = Color(0, 100, 20);
                }
            }
        }
    }

    // 把上面字符串最后的'\0'去掉

    for (int i = 0; i < MAP_HEIGHT; i++)
    {
        if (i < 15)
        {
            blocktype[i][MAP_LENGTH - 1].type = ' ';
        }
        else
        {
            blocktype[i][MAP_LENGTH - 1].type = BG;
        }
    }
}

void background::show()
{
    for (int i = 0; i < MAP_HEIGHT; i++)
    {
        for (int j = 0; j < MAP_LENGTH; j++)
        {
            printf("%c ", blocktype[i][j]);
        }
        printf("\n");
    }
}