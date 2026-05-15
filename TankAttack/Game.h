#pragma once
#include "Map.h"
#include "Renderer.h"
#include "Player.h"
#include "Pathfinding.h"


class Game
{
public:
    Game();
    void run();
    void handleInput();
    void render();
    void update();


private:
    Map map;
    Renderer renderer;

    BulletPath bulletTrail;
    void dealBulletDamage(const BulletPath& shot);

    // Jugadores
    Player player1;
    Player player2;

    int currentTurn = 1;

    Tank* selectedTank = nullptr;

    Path currentPath;

};

