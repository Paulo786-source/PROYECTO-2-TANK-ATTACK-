#pragma once
#include "Pathfinding.h"

// BFS o Dijkstra con su probabilidad de uso vs movimiento aleatorio
class PathfindingStrategy {
public:
    virtual ~PathfindingStrategy() = default;

    // bfsProb / dijkstraProb permiten que un power-up ajuste la probabilidad
    virtual Path calculatePath(
        const Map& map,
        Position origin,
        Position destination,
        const bool* blocked,
        int overrideProbability = -1   // usar la probabilidad por defecto
    ) const = 0;
};

//tanques azul/celeste: 50% BFS, 50% aleatorio.
class BFSStrategy : public PathfindingStrategy {
public:
    Path calculatePath(
        const Map& map,
        Position origin,
        Position destination,
        const bool* blocked,
        int overrideProbability = -1
    ) const override {
        int prob = (overrideProbability >= 0) ? overrideProbability : 50;
        return Pathfinding::calculatePath(map, origin, destination, true, blocked, prob, 80);
    }
};

// tanques amarillo/rojo: 80% Dijkstra, 20% aleatorio.
class DijkstraStrategy : public PathfindingStrategy {
public:
    Path calculatePath(
        const Map& map,
        Position origin,
        Position destination,
        const bool* blocked,
        int overrideProbability = -1
    ) const override {
        int prob = (overrideProbability >= 0) ? overrideProbability : 80;
        return Pathfinding::calculatePath(map, origin, destination, false, blocked, 50, prob);
    }
};