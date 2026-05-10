#include "Player.h"
#include "Tank.h"


Player::Player(int playerNumber) {
	this->playerNumber = playerNumber;
	if (playerNumber == 1) {
		tanks[0] = new Tank({ 0, 0 }, Tank::TankColor::blue);
		tanks[1] = new Tank({ 0, 1 }, Tank::TankColor::blue);
		tanks[2] = new Tank({ 1, 0 }, Tank::TankColor::red);
		tanks[3] = new Tank({ 1, 1 }, Tank::TankColor::red);
	}
	else {
		tanks[0] = new Tank({ ROWS - 1, COLS - 1 }, Tank::TankColor::cyan);
		tanks[1] = new Tank({ ROWS - 1, COLS - 2 }, Tank::TankColor::cyan);
		tanks[2] = new Tank({ ROWS - 2, COLS - 1 }, Tank::TankColor::yellow);
		tanks[3] = new Tank({ ROWS - 2, COLS - 2 }, Tank::TankColor::yellow);
	}
}

Player::~Player() {
	for (int i = 0; i < 4; i++) {
		delete tanks[i];
	}
}

Tank& Player::getTank(int index) {
	return *tanks[index];
}

int Player::getPlayerNumber() const {
	return playerNumber;
}

bool Player::hasTanks() const {
	for (int i = 0; i < 4; i++) {
		if (tanks[i]->isAlive() == true) {
			return true;
		}
	}

	return false;
}