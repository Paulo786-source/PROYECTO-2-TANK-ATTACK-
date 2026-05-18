#include "Player.h"
#include "Tank.h"
#include "PowerUpQueue.h"


Player::Player(int playerNumber) {
	this->playerNumber = playerNumber;
	if (playerNumber == 1) {
		tanks[0] = new Tank({ 0, 0 }, Tank::TankColor::blue);
		tanks[1] = new Tank({ 1, 0 }, Tank::TankColor::blue);
		tanks[2] = new Tank({ ROWS - 1, 0 }, Tank::TankColor::red);
		tanks[3] = new Tank({ ROWS - 2, 0 }, Tank::TankColor::red);
	}
	else {
		tanks[0] = new Tank({ 0, COLS - 1 }, Tank::TankColor::cyan);
		tanks[1] = new Tank({ 1, COLS - 1 }, Tank::TankColor::cyan);
		tanks[2] = new Tank({ ROWS - 1, COLS - 1 }, Tank::TankColor::yellow);
		tanks[3] = new Tank({ ROWS - 2, COLS - 1 }, Tank::TankColor::yellow);
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

void Player::addPowerUp(PowerUp p) {
	powerUps.enqueue(p);
}

// saca y retorna el primero de la cola
PowerUp Player::usePowerUp() {
	return powerUps.dequeue();
}

bool Player::hasPowerUps() const {
	return !powerUps.isEmpty();
}

// muestra cuál es el siguiente sin consumirlo
PowerUp Player::nextPowerUp() const {
	return powerUps.peek();
}

const PowerUpQueue& Player::getPowerUpQueue() const {
	return powerUps;
}