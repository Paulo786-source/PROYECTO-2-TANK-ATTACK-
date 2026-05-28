#include "Player.h"
#include "Tank.h"
#include "PowerUpQueue.h"
#include "TankFactory.h"


Player::Player(int playerNumber) {
	this->playerNumber = playerNumber;
	TankFactory::createTanks(playerNumber, tanks);
}

Player::~Player() {
	for (int i = 0; i < 4; i++) {
		delete tanks[i];
	}
}

Tank& Player::getTank(int index) {
	return *tanks[index];
}

const Tank& Player::getTank(int index) const {
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

// muestra cual es el siguiente sin consumirlo
PowerUp Player::nextPowerUp() const {
	return powerUps.peek();
}

const PowerUpQueue& Player::getPowerUpQueue() const {
	return powerUps;
}

int Player::getPowerUpCount() const {
	return powerUps.size();
}