#include "Pathfinding.h"
#include <cstdlib>
#include <cmath>
#include "Player.h"

Path Pathfinding::reconstructPath(const Map& map, int* parent, int originNode, int destNode) {
    Path path;

    // recorrer de destino a origen siguiendo los padres
    int current = destNode;

    while (current != -1) {
        path.nodes[path.length] = current;
        path.length++;
        current = parent[current];
    }

    // invertir el arreglo para que quede de origen a destino
    int left = 0;
    int right = path.length - 1;

    while (left < right) {
        int temp = path.nodes[left];
        path.nodes[left] = path.nodes[right];
        path.nodes[right] = temp;
        left++;
        right--;
    }

    return path;
}

bool Pathfinding::hasLineOfSight(const Map& map, Position a, Position b) {
    int rowDiff = b.row - a.row;
    int colDiff = b.col - a.col;

    // la cantidad de pasos es el mayor de los dos desplazamientos
    int steps = abs(rowDiff) > abs(colDiff) ? abs(rowDiff) : abs(colDiff);
    if (steps == 0) return true;

    for (int i = 1; i <= steps; i++) {
        // interpolar la posición en cada paso para trazar la línea
        // se suma 0.5f para no redondear hacia abajo
        int row = a.row + (int)((rowDiff * i) / (float)steps + 0.5f);
        int col = a.col + (int)((colDiff * i) / (float)steps + 0.5f);

        if (map.getCell(row, col).type == CellType::obstacle) {
            return false;
        }
    }
    return true;
}

// para tanques azul/celeste
Path Pathfinding::BFS(const Map& map, Position origin, Position destination) {
    int totalNodes = ROWS * COLS;
    int originNode = map.coordsToNode(origin.row, origin.col);
    int destNode = map.coordsToNode(destination.row, destination.col);
    bool visited[ROWS * COLS] = {};
    int  parent[ROWS * COLS];

    for (int i = 0; i < totalNodes; i++) parent[i] = -1;

    int queue[ROWS * COLS];
    int front = 0, back = 0;

    visited[originNode] = true;
    queue[back] = originNode;
    back++;

    while (front != back) {
        int current = queue[front];
        front++;

        if (current == destNode) break;

        int neighbors[4];
        int count = 0;
        map.getGraph()->getNeighbors(current, neighbors, count);

        for (int i = 0; i < count; i++) {
            int neighbor = neighbors[i];
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                parent[neighbor] = current;
                queue[back] = neighbor;
                back++;
            }
        }
    }

    if (!visited[destNode]) return Path(); // no hay camino posible

    return reconstructPath(map, parent, originNode, destNode);
}

// para tanques rojo/amarillo
Path Pathfinding::Dijkstra(const Map& map, Position origin, Position destination) {
    int totalNodes = ROWS * COLS;
    int originNode = map.coordsToNode(origin.row, origin.col);
    int destNode = map.coordsToNode(destination.row, destination.col);

    int  distance[ROWS * COLS];
    bool visited[ROWS * COLS] = {};
    int  parent[ROWS * COLS];

    // empiezan todas las distancias en infinito
    for (int i = 0; i < totalNodes; i++) {
        distance[i] = 999999;
        parent[i] = -1;
    }
    distance[originNode] = 0;

    for (int iter = 0; iter < totalNodes; iter++) {
        // buscar el nodo no visitado con menor distancia acumulada
        int current= -1;
        for (int i = 0; i < totalNodes; i++) {
            if (!visited[i] && (current== -1 || distance[i] < distance[current])) {
                current= i;
            }
        }

        if (current== -1 || distance[current] == 999999) break; // no quedan nodos alcanzables
        if (current== destNode) break;

        visited[current] = true;

        int neighbors[4];
        int count = 0;
        map.getGraph()->getNeighbors(current, neighbors, count);

        for (int i = 0; i < count; i++) {
            int neighbor = neighbors[i];
            int newDistance = distance[current] + 1; // cada celda tiene peso 1

            if (newDistance < distance[neighbor]) {
                distance[neighbor] = newDistance;
                parent[neighbor] = current;
            }
        }
    }

    if (distance[destNode] == 999999) return Path(); // no hay camino posible

    return reconstructPath(map, parent, originNode, destNode);
}

Path Pathfinding::advanceUntilObstacle(const Map& map, Position origin, Position destination) {
    Path path;
    int rowDiff = destination.row - origin.row;
    int colDiff = destination.col - origin.col;
    int steps = abs(rowDiff) > abs(colDiff) ? abs(rowDiff) : abs(colDiff);

    for (int i = 0; i <= steps; i++) {
        int row = origin.row + (int)((rowDiff * i) / (float)(steps == 0 ? 1 : steps) + 0.5f);
        int col = origin.col + (int)((colDiff * i) / (float)(steps == 0 ? 1 : steps) + 0.5f);

        // para si se topa con un obstáculo
        if (map.getCell(row, col).type == CellType::obstacle) break;

        path.nodes[path.length] = map.coordsToNode(row, col);
        path.length++;
    }
    return path;
}


// agrega las celdas de una línea recta entre dos posiciones a una ruta existente
void Pathfinding::addSegment(const Map& map, Path& path, Position from, Position to, bool skipFirst) {
    int rowDiff = to.row - from.row;
    int colDiff = to.col - from.col;
    int steps = abs(rowDiff) > abs(colDiff) ? abs(rowDiff) : abs(colDiff);

    // si skipFirst es true entonces empieza en 1 para no repetir el punto de inicio
    int start = skipFirst ? 1 : 0;

    for (int i = start; i <= steps; i++) {
        int row = from.row + (int)((rowDiff * i) / (float)(steps == 0 ? 1 : steps) + 0.5f);
        int col = from.col + (int)((colDiff * i) / (float)(steps == 0 ? 1 : steps) + 0.5f);

        if (map.getCell(row, col).type == CellType::obstacle) break;

        path.nodes[path.length] = map.coordsToNode(row, col);
        path.length++;
    }
}

Path Pathfinding::randomMovement(const Map& map, Position origin, Position destination) {
    // primero línea vista directa al destino
    if (hasLineOfSight(map, origin, destination)) {
        Path path;
        addSegment(map, path, origin, destination, false);
        return path;
    }

    // si no hay línea vista entonces se elige una celda libre aleatoria dentro del radio
    Position midpoint = origin;

    for (int attempt = 0; attempt < 10; attempt++) {
        int rowJump = (rand() % (2 * RANDOM_RADIUS + 1)) - RANDOM_RADIUS;
        int colJump = (rand() % (2 * RANDOM_RADIUS + 1)) - RANDOM_RADIUS;

        int newRow = origin.row + rowJump;
        int newCol = origin.col + colJump;

        bool inBounds = (newRow >= 0 && newRow < ROWS &&
            newCol >= 0 && newCol < COLS);

        if (inBounds && map.getCell(newRow, newCol).type == CellType::free) {
            midpoint = { newRow, newCol };
            break;
        }
    }

    // si no sirve, se intenta línea vista desde la intermedia al destino
    if (hasLineOfSight(map, midpoint, destination)) {
        Path path;
        addSegment(map, path, origin, midpoint, false);
        addSegment(map, path, midpoint, destination, true);
        return path;
    }

    // si ningún intento funcionó, se avanza hasta donde sea posible
    return advanceUntilObstacle(map, origin, destination);
}

// decide qué algoritmo usar según el tipo de tanque y un número aleatorio
Path Pathfinding::calculatePath(const Map& map, Position origin, Position destination, bool useBFS, int bfsProb, int dijkstraProb) {
    int probability = rand() % 100;

    if (useBFS) {
        if (probability < bfsProb) {
            return BFS(map, origin, destination);
        }
        else {
            return randomMovement(map, origin, destination);
        }
    }
    else {
        if (probability < dijkstraProb) {
            return Dijkstra(map, origin, destination);
        }
        else {
            return randomMovement(map, origin, destination);
        }
    }
}

// toma el vector entre origen y destino y lo simplifica a -1, 0 o 1
void Pathfinding::getInitialDirection(Position origin, Position target, int& dx, int& dy) {
    int rowDiff = target.row - origin.row;
    int colDiff = target.col - origin.col;

    // lo convierte en -1, 0, 1
    if (rowDiff != 0) dx = rowDiff / abs(rowDiff);
    else dx = 0;

    if (colDiff != 0) dy = colDiff / abs(colDiff);
    else dy = 0;
}

BulletPath Pathfinding::traceBulletWithBounces(const Map& map, Position origin, int dx, int dy, Player& player1, Player& player2) {
    BulletPath result;
    int currentRow = origin.row;
    int currentCol = origin.col;
    int bounces = 0;
    int moves = 0;
    int maxMoves = ROWS + COLS;
    int consecutiveBounces = 0;

    // agrega posición inicial
    result.nodes[result.length++] = map.coordsToNode(currentRow, currentCol);

    while (moves < maxMoves && bounces <= MAX_BOUNCES) {
        // si lleva muchos rebotes seguidos sin avanzar entonces está atascada
        if (consecutiveBounces > 8) break;

        int nextRow = currentRow + dx;
        int nextCol = currentCol + dy;

        bool outRow = (nextRow < 0 || nextRow >= ROWS);
        bool outCol = (nextCol < 0 || nextCol >= COLS);

        // si es esquina rebota en ambas direcciones
        if (outRow && outCol) {
            dx = -dx;
            dy = -dy;
            bounces++;
            consecutiveBounces++;
            continue;
        }

        // choca con borde horizontal, rebota en fila
        if (outRow) {
            dx = -dx;
            bounces++;
            consecutiveBounces++;
            continue;
        }

        // choca con borde vertical, rebota en columna
        if (outCol) {
            dy = -dy;
            bounces++;
            consecutiveBounces++;
            continue;
        }

        if (map.getCell(nextRow, nextCol).type == CellType::obstacle) {
            // determina si la pared es horizontal, vertical o esquina
            bool blockedRow = (currentRow + dx < 0 || currentRow + dx >= ROWS ||
                map.getCell(currentRow + dx, currentCol).type == CellType::obstacle);
            bool blockedCol = (currentCol + dy < 0 || currentCol + dy >= COLS ||
                map.getCell(currentRow, currentCol + dy).type == CellType::obstacle);

            if (blockedRow && blockedCol) {
                dx = -dx;
                dy = -dy;
            }
            else if (blockedRow) {
                dx = -dx;
            }
            else {
                dy = -dy;
            }
            bounces++;
            consecutiveBounces++;
            continue;
        }

        // si hay celda libre verifica si hay un tanque
        int nextNode = map.coordsToNode(nextRow, nextCol);
        for (int p = 1; p <= 2; p++) {
            Player& pl = (p == 1) ? player1 : player2;
            for (int t = 0; t < 4; t++) {
                Tank& tank = pl.getTank(t);
                if (tank.isAlive() &&
                    tank.getPosition().row == nextRow &&
                    tank.getPosition().col == nextCol) {
                    // registrar impacto
                    result.nodes[result.length++] = nextNode;
                    result.hitTank = true;
                    result.hitTankNode = nextNode;
                    return result;
                }
            }
        }

        // avanzar normal, solo aquí cuenta como movimiento real
        currentRow = nextRow;
        currentCol = nextCol;
        result.nodes[result.length++] = map.coordsToNode(currentRow, currentCol);
        moves++;
        consecutiveBounces = 0;
    }

    return result;
}

BulletPath Pathfinding::calculateBulletPath(const Map& map, Position origin, Position target, Player& player1, Player& player2) {
    int dx = 0, dy = 0;
    getInitialDirection(origin, target, dx, dy);

    // no hay dirección 
    if (dx == 0 && dy == 0) return BulletPath();

    return traceBulletWithBounces(map, origin, dx, dy, player1, player2);
}

// A* para el disparo de precisión
int Pathfinding::heuristic(Position a, Position b) {
    return abs(a.row - b.row) + abs(a.col - b.col);
}

BulletPath Pathfinding:: precisionShot(const Map& map, Position origin,
    Position target,
    Player& player1, Player& player2) {
    int totalNodes = ROWS * COLS;
    int originNode = map.coordsToNode(origin.row, origin.col);
    int destNode = map.coordsToNode(target.row, target.col);

    static int gCost[ROWS * COLS];
    static int fCost[ROWS * COLS];
    static int parent[ROWS * COLS];
    static bool visited[ROWS * COLS];
    static bool inQueue[ROWS * COLS];

    for (int i = 0; i < totalNodes; i++) {
        gCost[i] = 999999;
        fCost[i] = 999999;
        parent[i] = -1;
        visited[i] = false;
        inQueue[i] = false;
    }

    gCost[originNode] = 0;

    // convertir originNode a coordenadas para calcular h
    Position originPos = origin;
    fCost[originNode] = heuristic(originPos, target);
    inQueue[originNode] = true;

    for (int iter = 0; iter < totalNodes; iter++) {
        // buscar el nodo con menor fCost
        int current= -1;
        for (int i = 0; i < totalNodes; i++) {
            if (inQueue[i] && !visited[i]) {
                if (current== -1 || fCost[i] < fCost[current]) {
                    current= i;
                }
            }
        }

        if (current== -1) break;  // no hay camino
        if (current== destNode) break;   

        visited[current] = true;
        inQueue[current] = false;

        int neighbors[4];
        int count = 0;
        map.getGraph()->getNeighbors(current, neighbors, count);

        for (int i = 0; i < count; i++) {
            int neighbor = neighbors[i];
            if (visited[neighbor]) continue;

            int tentativeG = gCost[current] + 1;  // el peso g es igual que en Dijkstra

            if (tentativeG < gCost[neighbor]) {
                parent[neighbor] = current;
                gCost[neighbor] = tentativeG;

                // reconstruir position del vecino para calcular h
                int nRow, nCol;
                map.nodeToCoords(neighbor, nRow, nCol);
                Position neighborPos = { nRow, nCol };

                fCost[neighbor] = tentativeG + heuristic(neighborPos, target);
                inQueue[neighbor] = true;
            }
        }
    }

    // no se alcanzó el destino
    if (gCost[destNode] == 999999) return BulletPath();

    Path basePath = reconstructPath(map, parent, originNode, destNode);

    // copiar nodo por nodo a BulletPath
    BulletPath result;
    for (int i = 0; i < basePath.length && result.length < MAX_BULLET_STEPS; i++) {
        int node = basePath.nodes[i];
        result.nodes[result.length++] = node;

        if (i == 0) continue; // para que no se pegue a sí mismo quien dispara

        // verificar si hay tanque en el nodo
        int nRow, nCol;
        map.nodeToCoords(node, nRow, nCol);

        for (int p = 1; p <= 2; p++) {
            Player& pl = (p == 1) ? player1 : player2;
            for (int t = 0; t < 4; t++) {
                Tank& tank = pl.getTank(t);
                if (tank.isAlive() &&
                    tank.getPosition().row == nRow &&
                    tank.getPosition().col == nCol) {
                    result.hitTank = true;
                    result.hitTankNode = node;
                    return result;
                }
            }
        }
    }

    return result;
}

BulletPath Pathfinding::calculateprecisionShot(const Map& map, Position origin,
    Position target,
    Player& player1, Player& player2) {
    return precisionShot(map, origin, target, player1, player2);
}