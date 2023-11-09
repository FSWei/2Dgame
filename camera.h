#ifndef CAMERA_H
#define CAMERA_H

#include "def.h"

struct camera
{
    int left;
    int top;
    int width;
    int height;
    int updatetime = 200;//这个值越小帧率越高，但是注意不要太小，否则效果适得其反

    void init(int left, int top, int width, int height);//初始化
    void update(int playerX, int playerY);//根据player坐标更新camera位置
    bool isInsideCamera(int x, int y);//判断是否在camera里
    void renderObject(int x, int y, char object);//渲染
    void showBorder();//呈现边框（其实就是染个色）
};

#endif