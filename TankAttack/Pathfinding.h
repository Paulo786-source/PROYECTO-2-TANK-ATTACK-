#pragma once
#include "Map.h"

class Player;

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

struct BulletPath {
    int nodes[ROWS * COLS];
    int length;
    bool hitTank;
    int hitTankNode;
    BulletPath() : length(0), hitTank(false), hitTankNode(-1) {
        for (int i = 0; i < ROWS * COLS; i++) nodes[i] = 0;
    }
};

const int MAX_BULLET_STEPS = ROWS * COLS;
const int MAX_BOUNCES = 3;

// radio en celdas para el movimiento aleatorio 
const int RANDOM_RADIUS = 3;

class Pathfinding {
public:
    // tanques celeste/azul : 50 % BFS, 50 % aleatorio
    // tanques rojo / amarillo : 80 % Dijkstra, 20 % aleatorio
    static Path calculatePath(const Map& map, Position origin, Position destination, bool useBFS);

    static BulletPath calculateBulletPath(const Map& map, Position origin, Position target, Player& player1, Player& player2);

private:
    static Path advanceUntilObstacle(const Map& map, Position origin, Position destination);
    static Path BFS(const Map& map, Position origin, Position destination);
    static Path Dijkstra(const Map& map, Position origin, Position destination);
    static Path randomMovement(const Map& map, Position origin, Position destination);
    static Path reconstructPath(const Map& map, int* parent, int originNode, int destNode);
    static bool hasLineOfSight(const Map& map, Position a, Position b);
    static void addSegment(const Map& map, Path& path, Position from, Position to, bool skipFirst);
    
    // dirección del disparo
    static BulletPath traceBulletWithBounces(const Map& map, Position origin, int dx, int dy, Player& player1, Player& player2);
    static void getInitialDirection(Position origin, Position target, int& dx, int& dy);
};