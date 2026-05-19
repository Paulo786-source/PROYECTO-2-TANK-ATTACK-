#include "Tank.h"

Tank::Tank(Position pos, TankColor color) {
	this->pos = pos;
	this->color = color;

	if (color == TankColor::blue || color == TankColor::cyan) {
		type = TankType::BFS;
	}

	else {
		type = TankType::Dijkstra;
	}
}

void Tank::receiveDamage(int damageReceived) {
	health -= damageReceived;

	if (health <= 0) {
		health = 0;
		alive = false;
	}
}

//Setters

void Tank::setSelected(bool value) {
	selected = value;
}
void Tank::setPosition(Position pos) {
	this-> pos = pos;
}
void Tank::setAngle(float angle) {
	this->angle = angle;
}

//Getters

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

