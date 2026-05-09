#pragma once
#include "Map.h"

struct Position {
    int row;
    int col;
};

struct Path {
    int nodes[ROWS * COLS];
    int length;
    Path() : length(0) {
        for (int i = 0; i < ROWS * COLS; i++) nodes[i] = 0;
    }
};

// radio en celdas para el movimiento aleatorio 
const int RANDOM_RADIUS = 3;

class Pathfinding {
public:
    // tanques celeste/azul : 50 % BFS, 50 % aleatorio
    // tanques rojo / amarillo : 80 % Dijkstra, 20 % aleatorio
    static Path calculatePath(const Map& map, Position origin, Position destination, bool useBFS);

private:
    static Path advanceUntilObstacle(const Map& map, Position origin, Position destination);
    static Path BFS(const Map& map, Position origin, Position destination);
    static Path Dijkstra(const Map& map, Position origin, Position destination);
    static Path randomMovement(const Map& map, Position origin, Position destination);
    static Path reconstructPath(const Map& map, int* parent, int originNode, int destNode);
    static bool hasLineOfSight(const Map& map, Position a, Position b);
    static void addSegment(const Map& map, Path& path, Position from, Position to, bool skipFirst);
};