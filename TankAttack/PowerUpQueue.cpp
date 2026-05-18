#include "PowerUpQueue.h"

PowerUpQueue::PowerUpQueue() : front(0), back(0), count(0) {}

void PowerUpQueue::enqueue(PowerUp p) {
    if (count >= MAX_POWERUPS) return;
    items[back] = p;
    // avanza el indice back de forma circular
    back = (back + 1) % MAX_POWERUPS;
    count++;
}

PowerUp PowerUpQueue::dequeue() {
    PowerUp p = items[front];
    // avanza el indice front de forma circular
    front = (front + 1) % MAX_POWERUPS;
    count--;
    return p;
}

// retorna el siguiente sin sacarlo
PowerUp PowerUpQueue::peek() const {
    return items[front];
}

bool PowerUpQueue::isEmpty() const {
    return count == 0;
}

int PowerUpQueue::size() const {
    return count;
}