//输入处理

#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include "player.h"

class inputhandler
{
public:
    void handleInput(player &p);
    bool isKeyPressed(int keyCode);
};

#endif
