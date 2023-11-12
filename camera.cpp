#include "camera.h"

void camera::init(int left, int top, int width, int height)
{
    this->left = left;
    this->top = top;
    this->width = width;
    this->height = height;

    if (this->left < 0)
        this->left = 0;
    if (this->top < 0)
        this->top = 0;
    if (this->left + width >= MAP_LENGTH)
        this->left = MAP_LENGTH - width;
    if (this->top + height >= MAP_HEIGHT)
        this->top = MAP_HEIGHT - height;
}

void camera::update(int playerX, int playerY)
{
    // 根据角色的位置更新镜头的位置
    left = playerX - width / 2;
    top = playerY - height / 2;

    if (left < 0)
        left = 0;
    if (top < 0)
        top = 0;
    if (left + width >= MAP_LENGTH)
        left = MAP_LENGTH - width;
    if (top + height >= MAP_HEIGHT)
        top = MAP_HEIGHT - height;
}

bool camera::isInsideCamera(int x, int y)
{
    return x >= left && x < left + width && y >= top && y < top + height;
}

void camera::renderObject(int x, int y, char object)
{
    if (isInsideCamera(x, y))
    {
        // gotoxy((LEFT_BORDER + (x - left)) * 2, TOP_BORDER + (y - top));
        move(TOP_BORDER + (y - top), (LEFT_BORDER + (x - left)) * 2);
        printw("%c ", object);
    }
}

void camera::showBorder()
{
    SetColor(Color(BORDER_COLOR), Color(BORDER_COLOR));
    // 上面
    for (int i = 0; i < TOP_BORDER; i++)
    {
        // gotoxy(0, i);
        move(i, 0);
        for (int j = 0; j < LEFT_BORDER + CAMERA_LENGTH + RIGHT_BORDER; j++)
        {
            printw("  ");
        }
    }

    // 下面
    for (int i = TOP_BORDER + CAMERA_HEIGHT; i < TOP_BORDER + CAMERA_HEIGHT + BOTTOM_BORDER; i++)
    {
        // gotoxy(0, i);
        move(i, 0);
        for (int j = 0; j < LEFT_BORDER + CAMERA_LENGTH + RIGHT_BORDER; j++)
        {
            printw("  ");
        }
    }

    // 左边
    for (int i = TOP_BORDER; i < TOP_BORDER + CAMERA_HEIGHT; i++)
    {
        // gotoxy(0, i);
        move(i, 0);
        for (int j = 0; j < LEFT_BORDER; j++)
        {
            printw("  ");
        }
    }

    // 右边
    for (int i = TOP_BORDER; i < TOP_BORDER + CAMERA_HEIGHT; i++)
    {
        // gotoxy((LEFT_BORDER + CAMERA_LENGTH) * 2, i);
        move(i, (LEFT_BORDER + CAMERA_LENGTH) * 2);
        for (int j = LEFT_BORDER + CAMERA_LENGTH; j < LEFT_BORDER + CAMERA_LENGTH + RIGHT_BORDER; j++)
        {
            printw("  ");
        }
    }
    UnsetColor();
}