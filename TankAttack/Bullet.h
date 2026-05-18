#pragma once
#include "Pathfinding.h"
#include "Tank.h"


class Bullet
{
private:
	
	BulletPath trail;
	Tank* shooter;
	bool fullPower;

public:

	Bullet(BulletPath trail, Tank* shooter, bool fullPower);

	const BulletPath& getBulletPath() const;
	Tank* getShooter() const;
	bool isFullPower() const;

};

