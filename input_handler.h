//输入处理

#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include "player.h"

struct Inputhandler
{
public:
    void handleInput(Player &player);
    bool isKeyPressed(int keyCode);
};

#endif
