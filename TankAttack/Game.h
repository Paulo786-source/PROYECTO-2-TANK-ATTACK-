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
    Texture2D hub;
    Bullet* currentBullet = nullptr;

    // jugadores
    Player player1;
    Player player2;
    int currentTurn = 1;

    Tank* selectedTank = nullptr;
    Path currentPath;

    // el último power-up activado con shift
    PowerUp activePowerUp = { PowerUpType::doubleTurn };

    // true si hay un power-up pendiente de aplicar en el próximo turno del dueño
    bool hasPendingPowerUp = false;

    // jugador al que pertenece el power-up pendiente
    int pendingPowerUpOwner = 0;
    const char* powerUpMessage1 = "";
    const char* powerUpMessage2 = "";
    int extraTurns = 0;

    // turnos extra que esperan activarse cuando le toque al dueño
    int pendingExtraTurns = 0;
    int doubleTurnOwner = 0;

    bool gameOver = false;
    float timeRemaining = 300.0f;
    bool timeUp = false;

    AudioManager audio;
    GameResult result = GameResult::None;

    void nextTurn();
    void randomPowerUp();
    void checkEliminationWin();
    void checkTimeWin();
};