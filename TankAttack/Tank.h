#pragma once
#include "Pathfinding.h"
#include "PathfindingStrategy.h"

class Tank
{
public:

    enum class TankColor {
        blue,
        cyan,
        red,
        yellow
    };

    enum class TankType {
        BFS,
        Dijkstra
    };

    Tank(Position pos, TankColor color);
    ~Tank();

    // set
    void setSelected(bool value);
    void setPosition(Position pos);
    void setAngle(float angle);

    // get
    int getHealth() const;
    bool isAlive() const;
    bool isSelected() const;
    Position getPosition() const;
    TankColor getTankColor() const;
    TankType getTankType() const;
    float getAngle() const;
    const PathfindingStrategy* getStrategy() const;

    void receiveDamage(int damageReceived);

private:

    Position pos;

    TankType type;

    TankColor color;

    int health = 100;

    bool selected = false;

    bool alive = true;

    float angle = 0.0f;

    PathfindingStrategy* strategy;
};