#pragma once

// tipos de power-up disponibles
enum class PowerUpType {
    doubleTurn,      
    movePrecision,   // BFS/Dijkstra con 90% de probabilidad
    attackPrecision,
    attackPower    
};

struct PowerUp {
    PowerUpType type;
};

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