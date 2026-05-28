#pragma once
#include "Map.h"
#include "Renderer.h"
#include "Player.h"
#include "Pathfinding.h"
#include "Bullet.h"
#include "AudioManager.h"
#include "GameEventListener.h"

// para Command Pattern
class MoveCommand;
class ShootCommand;
class PowerUpCommand;

enum class GameResult { None, Player1Wins, Player2Wins, Draw };

class Game
{
public:
    Game();
    ~Game();
    void run();
    void handleInput();
    void render();
    void update();
    void dealBulletDamage(const Bullet& bullet);

    // Observer: notifica a todos los listeners registrados
    void notify(GameEvent event);

private:
    Map map;
    Renderer renderer;
    Bullet* currentBullet = nullptr;

    // jugadores
    Player player1;
    Player player2;
    int currentTurn = 1;

    Tank* selectedTank = nullptr;
    Path currentPath;

    // power-up pendiente por jugador (indices 1 y 2; el 0 no se usa)
    PowerUp pendingPowerUp[3] = {};
    bool hasPendingPowerUp[3] = { false, false, false };

    const char* powerUpMessage1 = "";
    const char* powerUpMessage2 = "";
    int extraTurns = 0;

    // turnos extra que esperan activarse cuando le toque al due?o
    int pendingExtraTurns = 0;
    int doubleTurnOwner = 0;

    bool gameOver = false;
    float timeRemaining = 300.0f;
    bool timeUp = false;

    AudioManager audio;
    GameResult result = GameResult::None;

    // para observer
    GameEventListener* listeners[8];
    int listenerCount = 0;

    // helper para construir el blocked array
    void buildBlockedArray(bool* blocked) const;

    void nextTurn();
    void randomPowerUp();
    void checkEliminationWin();
    void checkTimeWin();

    // los comandos acceden a miembros privados de Game directamente (para eso sirve friend class)
    friend class MoveCommand;
    friend class ShootCommand;
    friend class PowerUpCommand;
};