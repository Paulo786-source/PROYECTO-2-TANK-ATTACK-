#pragma once
#include "Tank.h"
#include "PowerUpQueue.h"
#include "TankFactory.h"

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
    const Tank& getTank(int index) const;

    int getPlayerNumber() const;

    bool hasTanks() const;

    void addPowerUp(PowerUp p);
    PowerUp usePowerUp();
    bool hasPowerUps() const;
    PowerUp nextPowerUp() const;

    const PowerUpQueue& getPowerUpQueue() const;

    int getPowerUpCount() const;
};