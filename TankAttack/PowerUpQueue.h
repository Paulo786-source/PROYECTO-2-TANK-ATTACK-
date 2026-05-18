#pragma once

// tipos de power-up disponibles
enum class PowerUpType {
    doubleTurn,      // los siguientes dos turnos son del mismo jugador
    movePrecision,   // BFS/Dijkstra con 90% de probabilidad
    attackPrecision, // la bala sigue A* hacia el objetivo
    attackPower      // bala hace 100% de daño
};

struct PowerUp {
    PowerUpType type;
};

// maximo de power-ups que puede tener un jugador a la vez
const int MAX_POWERUPS = 10;

// cola circular para no desperdiciar espacio al desencolar

class PowerUpQueue {

public:
    PowerUpQueue();
    void enqueue(PowerUp p);
    PowerUp dequeue();
    PowerUp peek() const;
    bool isEmpty() const;
    int size() const;
    PowerUp getItem(int index) const;
    int getSize() const;

private:
    PowerUp items[MAX_POWERUPS];
    int front;
    int back;
    int count;
};