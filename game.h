#ifndef GAME_H
#define GAME_H

#include "def.h"
#include "player.h"
#include "enemy.h"
#include "map.h"
#include "camera.h"
#include "input_handler.h"

struct Game
{
public:
    Game();

    void run();

private:
    Camera *camera;
    Map *map;
    Background *background;
    Player *player;
    Inputhandler *inputhandler;
    Enemy *enemy;

    void init();
#ifdef __linux__
    void *keyboardCheck();
#endif
    void *Input(Inputhandler *ih, Player *player);
    void *updatePlayer(Player *player);
    void *updateEnemy(Enemy (**e));
    void *render(Player *player, Enemy (**e), Camera *camera);
    void print_data(Player *player, Enemy (**e));
    void *timer(Player *player);
    void end();
    void win();
    void lose();
};

#endif // GAME_H
