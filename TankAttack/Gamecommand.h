#pragma once
#include "Pathfinding.h"
#include "Bullet.h"
#include "Player.h"
#include "PowerUpQueue.h"

class Game;

class GameCommand {
public:
    virtual ~GameCommand() = default;
    virtual void execute(Game& game) = 0;
};

// mueve selectedTank a destination usando su estrategia
class MoveCommand : public GameCommand {
public:
    MoveCommand(Tank* tank, Position destination, bool usePrecision)
        : tank(tank), destination(destination), usePrecision(usePrecision) {
    }

    void execute(Game& game) override;

private:
    Tank* tank;
    Position destination;
    bool usePrecision;
};

// dispara desde origin hacia target
class ShootCommand : public GameCommand {
public:
    ShootCommand(Tank* tank, Position target, bool usePrecisionShot, bool fullPower)
        : tank(tank), target(target), usePrecisionShot(usePrecisionShot), fullPower(fullPower) {
    }

    void execute(Game& game) override;

private:
    Tank* tank;
    Position target;
    bool usePrecisionShot;
    bool fullPower;
};

// activa el primer power-up de la cola del jugador
class PowerUpCommand : public GameCommand {
public:
    explicit PowerUpCommand(int playerTurn) : playerTurn(playerTurn) {}

    void execute(Game& game) override;

private:
    int playerTurn;
};