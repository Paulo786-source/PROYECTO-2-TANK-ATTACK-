#include "Game.h"
#include <cstdlib>
#include <ctime>

int main() {
    srand(time(nullptr));
    Game game;
    game.run();
    return 0;
}