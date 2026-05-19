#pragma once
#include "Map.h"
#include "Renderer.h"
#include "Player.h"
#include "Pathfinding.h"
#include "Bullet.h"
#include "AudioManager.h"

enum class GameResult { None, Player1Wins, Player2Wins, Draw };

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

    const char* powerUpMessage1 = "";
    const char* powerUpMessage2 = "";

    int extraTurns = 0; // por power-up doble turno

    bool gameOver = false;
    float timeRemaining = 300.0f; // 5 minutos en segundos
    bool timeUp = false;

    AudioManager audio;

    GameResult result = GameResult::None;

    void nextTurn();

    void randomPowerUp();

    void checkEliminationWin();

    void checkTimeWin();
};

