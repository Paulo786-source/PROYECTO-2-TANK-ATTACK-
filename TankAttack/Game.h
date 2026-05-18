#pragma once
#include "Map.h"
#include "Renderer.h"
#include "Player.h"
#include "Pathfinding.h"
#include "Bullet.h"


class Game
{
public:
    Game();
    void run();
    void handleInput();
    void render();
    void update();
    void dealBulletDamage(const Bullet& bullet);


private:
    Map map;
    Renderer renderer;

    Bullet* currentBullet = nullptr;

    // Jugadores
    Player player1;
    Player player2;

    int currentTurn = 1;

    Tank* selectedTank = nullptr;

    Path currentPath;

    PowerUp activePowerUp = { PowerUpType::doubleTurn };
    bool hasPendingPowerUp = false;

    const char* powerUpMessage = "";

    int extraTurns = 0; // por power-up doble turno
    void nextTurn();
};

