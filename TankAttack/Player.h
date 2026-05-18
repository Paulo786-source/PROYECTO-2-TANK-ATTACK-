#pragma once
#include "Tank.h"
#include "PowerUpQueue.h"

class Player
{
private:

    int playerNumber;

    Tank* tanks[4];

    PowerUpQueue powerUps;

public:

    Player(int playerNumber);

    ~Player();

    Tank& getTank(int index);

    int getPlayerNumber() const;

    bool hasTanks() const;

    void addPowerUp(PowerUp p);
    PowerUp usePowerUp();
    bool hasPowerUps() const;
    PowerUp nextPowerUp() const;

    const PowerUpQueue& getPowerUpQueue() const;

    int getPowerUpCount() const;
};

