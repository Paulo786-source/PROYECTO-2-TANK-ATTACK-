#pragma once
#include "Tank.h"


class Player
{
private:
	
	int playerNumber;

	Tank tanks[4];

public:

	Player(int playerNumber);

	Tank& getTank(int index);

	int getPlayerNumber() const;

	bool hasTanks() const;
};

