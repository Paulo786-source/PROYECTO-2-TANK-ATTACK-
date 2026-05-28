#include "Bullet.h"

Bullet::Bullet(BulletPath trail, Tank* shooter, bool fullPower) {
	this -> trail = trail;
	this -> shooter = shooter;
	this -> fullPower = fullPower;
}

const BulletPath& Bullet::getBulletPath() const {
	return trail;
}
Tank* Bullet::getShooter() const {
	return shooter;
}
bool Bullet::isFullPower() const {
	return fullPower;
}
