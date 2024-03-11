#include "input_handler.h"

void Inputhandler::handleInput(Player &player)
{
    if (isKeyPressed(UPKEY))
    {
        player.isJumping = true;
    }

    if (isKeyPressed(RIGHTKEY))
    {
        player.isWalking = true;
        player.xdirection = RIGHT;
    }

    else if (isKeyPressed(LEFTKEY))
    {
        player.isWalking = true;
        player.xdirection = LEFT;
    }

    if (isKeyPressed(DASHKEY))
    {
        player.isdash = true;
    }

    // 如果左键右键a键d键都松开，isWalking = false
    if (!(isKeyPressed(LEFTKEY) || isKeyPressed(RIGHTKEY)))
    {
        player.isWalking = false;
    }
    // 如果上键松开，isJumping = false
    if (!isKeyPressed(UPKEY))
    {
        player.isJumping = false;
    }
    // 如果j键松开，isdash=false
    if (!isKeyPressed(DASHKEY))
    {
        player.isdash = false;
    }
}

bool Inputhandler::isKeyPressed(int keyCode)
{
#ifdef _WIN32
    // Windows 平台下的键盘状态检查代码
    if (keyCode == UPKEY)
        return (GetAsyncKeyState(0x57) & 0x8000 || GetAsyncKeyState(0x26) & 0x8000);
    else if (keyCode == LEFTKEY)
        return (GetAsyncKeyState(0x41) & 0x8000 || GetAsyncKeyState(0x25) & 0x8000);
    else if (keyCode == RIGHTKEY)
        return (GetAsyncKeyState(0x44) & 0x8000 || GetAsyncKeyState(0x27) & 0x8000);
    else if (keyCode == DASHKEY)
        // return (GetAsyncKeyState(0x4A) & 0x8000);
        return (GetAsyncKeyState(0x4C) & 0x8000);

#elif __linux__
    // Linux 平台下的键盘状态检查代码
    if (keyCode == UPKEY)
        return (key_state[17] || key_state[103]);
    else if (keyCode == LEFTKEY)
        return (key_state[30] || key_state[105]);
    else if (keyCode == RIGHTKEY)
        return (key_state[32] || key_state[106]);
    else if (keyCode == DASHKEY)
        // J
        //  return (key_state[36]);
        // L
        return (key_state[38])

#endif
    return false;
}