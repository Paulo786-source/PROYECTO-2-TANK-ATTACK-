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

    // interpolar la posición en cada paso para trazar la línea

    for (int i = 1; i <= steps; i++) {
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
Path Pathfinding::BFS(const Map& map, Position origin, Position destination, const bool* blocked) {
    int totalNodes = ROWS * COLS;
    int originNode = map.coordsToNode(origin.row, origin.col);
    int destNode = map.coordsToNode(destination.row, destination.col);
    bool visited[ROWS * COLS] = {};
    int parent[ROWS * COLS];

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
            // no pasar por celdas ocupadas por tanques
            if (blocked[neighbor]) continue;
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                parent[neighbor] = current;
                queue[back] = neighbor;
                back++;
            }
        }
    }

    if (!visited[destNode]) return Path();
    return reconstructPath(map, parent, originNode, destNode);
}

// para tanques rojo/amarillo
Path Pathfinding::Dijkstra(const Map& map, Position origin, Position destination, const bool* blocked) {
    int totalNodes = ROWS * COLS;
    int originNode = map.coordsToNode(origin.row, origin.col);
    int destNode = map.coordsToNode(destination.row, destination.col);

    int distance[ROWS * COLS];
    bool visited[ROWS * COLS] = {};
    int parent[ROWS * COLS];

    for (int i = 0; i < totalNodes; i++) {
        distance[i] = 999999;
        parent[i] = -1;
    }
    distance[originNode] = 0;

    for (int iter = 0; iter < totalNodes; iter++) {
        int u = -1;
        for (int i = 0; i < totalNodes; i++) {
            if (!visited[i] && (u == -1 || distance[i] < distance[u])) {
                u = i;
            }
        }

        if (u == -1 || distance[u] == 999999) break;
        if (u == destNode) break;

        visited[u] = true;

        int neighbors[4];
        int count = 0;
        map.getGraph()->getNeighbors(u, neighbors, count);

        for (int i = 0; i < count; i++) {
            int neighbor = neighbors[i];
            // no pasar por celdas ocupadas por tanques
            if (blocked[neighbor]) continue;
            int newDistance = distance[u] + 1;
            if (newDistance < distance[neighbor]) {
                distance[neighbor] = newDistance;
                parent[neighbor] = u;
            }
        }
    }

    if (distance[destNode] == 999999) return Path();
    return reconstructPath(map, parent, originNode, destNode);
}

Path Pathfinding::advanceUntilObstacle(const Map& map, Position origin, Position destination, const bool* blocked) {
    Path path;

    int row = origin.row;
    int col = origin.col;

    // punto de partida
    path.nodes[path.length++] = map.coordsToNode(row, col);

    int rowStep = 0, colStep = 0;
    if (destination.row > row) rowStep = 1;
    else if (destination.row < row) rowStep = -1;
    if (destination.col > col) colStep = 1;
    else if (destination.col < col) colStep = -1;

    // avanzar primero en filas, luego en columnas 
    while (row != destination.row) {
        int nextRow = row + rowStep;
        if (map.getCell(nextRow, col).type == CellType::obstacle) return path;
        if (blocked[map.coordsToNode(nextRow, col)]) return path;
        row = nextRow;
        path.nodes[path.length++] = map.coordsToNode(row, col);
    }
    while (col != destination.col) {
        int nextCol = col + colStep;
        if (map.getCell(row, nextCol).type == CellType::obstacle) return path;
        if (blocked[map.coordsToNode(row, nextCol)]) return path;
        col = nextCol;
        path.nodes[path.length++] = map.coordsToNode(row, col);
    }

    return path;
}


// blocked puede ser nullptr si no se necesita verificar tanques
void Pathfinding::addSegment(const Map& map, Path& path, Position from, Position to, bool skipFirst, const bool* blocked) {
    int row = from.row;
    int col = from.col;

    int rowStep = 0, colStep = 0;
    if (to.row > row) rowStep = 1;
    else if (to.row < row) rowStep = -1;
    if (to.col > col) colStep = 1;
    else if (to.col < col) colStep = -1;

    // agregar punto de inicio si no se omite
    if (!skipFirst) {
        path.nodes[path.length++] = map.coordsToNode(row, col);
    }

    // avanzar en filas primero
    while (row != to.row) {
        row += rowStep;
        if (map.getCell(row, col).type == CellType::obstacle) return;
        if (blocked && blocked[map.coordsToNode(row, col)]) return;
        path.nodes[path.length++] = map.coordsToNode(row, col);
    }
    // luego en columnas
    while (col != to.col) {
        col += colStep;
        if (map.getCell(row, col).type == CellType::obstacle) return;
        if (blocked && blocked[map.coordsToNode(row, col)]) return;
        path.nodes[path.length++] = map.coordsToNode(row, col);
    }
}


Path Pathfinding::randomMovement(const Map& map, Position origin, Position destination, const bool* blocked) {
    // primero línea vista directa al destino
    if (hasLineOfSight(map, origin, destination)) {
        Path path;
        addSegment(map, path, origin, destination, false, blocked);

        // verificar que addSegment llegó completamente al destino
        if (path.length > 0) {
            int lastRow, lastCol;
            map.nodeToCoords(path.nodes[path.length - 1], lastRow, lastCol);
            if (lastRow == destination.row && lastCol == destination.col)
                return path;
        }
    }

    // si no hay línea vista entonces se elige una celda libre aleatoria dentro del radio
    Position midpoint = origin;

    for (int attempt = 0; attempt < 10; attempt++) {
        int rowJump = (rand() % (2 * RANDOM_RADIUS + 1)) - RANDOM_RADIUS;
        int colJump = (rand() % (2 * RANDOM_RADIUS + 1)) - RANDOM_RADIUS;

        int newRow = origin.row + rowJump;
        int newCol = origin.col + colJump;

        bool inBounds = (newRow >= 0 && newRow < ROWS && newCol >= 0 && newCol < COLS);

        if (inBounds && map.getCell(newRow, newCol).type == CellType::free &&
            !blocked[map.coordsToNode(newRow, newCol)]) {
            midpoint = { newRow, newCol };
            break;
        }
    }

    // si hay línea vista desde la intermedia al destino
    if (hasLineOfSight(map, midpoint, destination)) {
        Path path;
        addSegment(map, path, origin, midpoint, false, blocked);
        addSegment(map, path, midpoint, destination, true, blocked);
        if (path.length > 0) {
            int lastRow, lastCol;
            map.nodeToCoords(path.nodes[path.length - 1], lastRow, lastCol);
            if (lastRow == destination.row && lastCol == destination.col)
                return path;
        }
    }

    // si ningún intento funcionó, se avanza hasta donde sea posible
    return advanceUntilObstacle(map, origin, destination, blocked);
}

// decide qué algoritmo usar según el tipo de tanque y un número aleatorio
Path Pathfinding::calculatePath(const Map& map, Position origin, Position destination, bool useBFS, const bool* blocked, int bfsProb, int dijkstraProb) {
    int probability = rand() % 100;

    if (useBFS) {
        if (probability < bfsProb)
            return BFS(map, origin, destination, blocked);
        else
            return randomMovement(map, origin, destination, blocked);
    }
    else {
        if (probability < dijkstraProb)
            return Dijkstra(map, origin, destination, blocked);
        else
            return randomMovement(map, origin, destination, blocked);
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
    int maxMoves = ROWS * COLS;
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

        // si va diagonal y hay paredes en ambos lados, rebota en esquina
        if (dx != 0 && dy != 0) {
            bool blockedRow = (currentRow + dx < 0 || currentRow + dx >= ROWS ||
                map.getCell(currentRow + dx, currentCol).type == CellType::obstacle);
            bool blockedCol = (currentCol + dy < 0 || currentCol + dy >= COLS ||
                map.getCell(currentRow, currentCol + dy).type == CellType::obstacle);

            if (blockedRow && blockedCol) {
                dx = -dx;
                dy = -dy;
                bounces++;
                consecutiveBounces++;
                continue;
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

BulletPath Pathfinding::precisionShot(const Map& map, Position origin,
    Position target,
    Player& player1, Player& player2) {
    int totalNodes = ROWS * COLS;
    int originNode = map.coordsToNode(origin.row, origin.col);
    int destNode = map.coordsToNode(target.row, target.col);

    // arrays locales en lugar de static para evitar estado residual entre llamadas
    int gCost[ROWS * COLS];
    int fCost[ROWS * COLS];
    int parent[ROWS * COLS];
    bool visited[ROWS * COLS];
    bool inQueue[ROWS * COLS];

    for (int i = 0; i < totalNodes; i++) {
        gCost[i] = 999999;
        fCost[i] = 999999;
        parent[i] = -1;
        visited[i] = false;
        inQueue[i] = false;
    }

    gCost[originNode] = 0;
    fCost[originNode] = heuristic(origin, target);
    inQueue[originNode] = true;

    for (int iter = 0; iter < totalNodes; iter++) {
        // buscar el nodo con menor fCost
        int current = -1;
        for (int i = 0; i < totalNodes; i++) {
            if (inQueue[i] && !visited[i]) {
                if (current == -1 || fCost[i] < fCost[current])
                    current = i;
            }
        }

        if (current == -1) break;
        if (current == destNode) break;

        visited[current] = true;
        inQueue[current] = false;

        int neighbors[4];
        int count = 0;
        map.getGraph()->getNeighbors(current, neighbors, count);

        for (int i = 0; i < count; i++) {
            int neighbor = neighbors[i];
            if (visited[neighbor]) continue;

            // no atravesar tanques intermedios (solo el destino puede tener un tanque)
            if (neighbor != destNode) {
                int nRow, nCol;
                map.nodeToCoords(neighbor, nRow, nCol);
                bool hasTank = false;
                for (int p = 1; p <= 2 && !hasTank; p++) {
                    Player& pl = (p == 1) ? player1 : player2;
                    for (int t = 0; t < 4 && !hasTank; t++) {
                        Tank& tk = pl.getTank(t);
                        if (tk.isAlive() && tk.getPosition().row == nRow && tk.getPosition().col == nCol)
                            hasTank = true;
                    }
                }
                if (hasTank) continue;
            }

            int tentativeG = gCost[current] + 1;
            if (tentativeG < gCost[neighbor]) {
                parent[neighbor] = current;
                gCost[neighbor] = tentativeG;
                int nRow, nCol;
                map.nodeToCoords(neighbor, nRow, nCol);
                fCost[neighbor] = tentativeG + heuristic({ nRow, nCol }, target);
                inQueue[neighbor] = true;
            }
        }
    }

    // no se alcanzó el destino
    if (gCost[destNode] == 999999) return BulletPath();

    Path basePath = reconstructPath(map, parent, originNode, destNode);

    // copiar nodo por nodo a BulletPath y detectar el primer tanque golpeado
    BulletPath result;
    for (int i = 0; i < basePath.length && result.length < MAX_BULLET_STEPS; i++) {
        int node = basePath.nodes[i];
        result.nodes[result.length++] = node;

        if (i == 0) continue; // no golpear al que dispara

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