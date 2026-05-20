#include "Tank.h"
#include "PathfindingStrategy.h"

Tank::Tank(Position pos, TankColor color) {
	this->pos = pos;
	this->color = color;

	if (color == TankColor::blue || color == TankColor::cyan) {
		type = TankType::BFS;
		strategy = new BFSStrategy();
	}
	else {
		type = TankType::Dijkstra;
		strategy = new DijkstraStrategy();
	}
}

Tank::~Tank() {
	delete strategy;
}

const PathfindingStrategy* Tank::getStrategy() const {
	return strategy;
}

void Tank::receiveDamage(int damageReceived) {
	health -= damageReceived;

	if (health <= 0) {
		health = 0;
		alive = false;
	}
}

// set

void Tank::setSelected(bool value) {
	selected = value;
}
void Tank::setPosition(Position pos) {
	this->pos = pos;
}
void Tank::setAngle(float angle) {
	this->angle = angle;
}

// get

int Tank::getHealth() const {
	return health;

}
bool Tank::isAlive() const {
	return alive;
}
bool Tank::isSelected() const {
	return selected;
}
Position Tank::getPosition() const {
	return pos;
}
Tank::TankColor Tank::getTankColor() const {
	return color;
}
Tank::TankType Tank::getTankType() const {
	return type;
}
float Tank::getAngle() const {
	return angle;
}