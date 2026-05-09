#include "Pathfinding.h"
#include <cstdlib>
#include <cmath>

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
        int u = -1;
        for (int i = 0; i < totalNodes; i++) {
            if (!visited[i] && (u == -1 || distance[i] < distance[u])) {
                u = i;
            }
        }

        if (u == -1 || distance[u] == 999999) break; // no quedan nodos alcanzables
        if (u == destNode) break;

        visited[u] = true;

        int neighbors[4];
        int count = 0;
        map.getGraph()->getNeighbors(u, neighbors, count);

        for (int i = 0; i < count; i++) {
            int neighbor = neighbors[i];
            int newDistance = distance[u] + 1; // cada celda tiene peso 1

            if (newDistance < distance[neighbor]) {
                distance[neighbor] = newDistance;
                parent[neighbor] = u;
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
Path Pathfinding::calculatePath(const Map& map, Position origin, Position destination, bool useBFS) {
    int probability = rand() % 100;

    if (useBFS) {
        if (probability < 50) {
            return BFS(map, origin, destination);
        }
        else {
            return randomMovement(map, origin, destination);
        }
    }
    else {
        if (probability < 80) {
            return Dijkstra(map, origin, destination);
        }
        else {
            return randomMovement(map, origin, destination);
        }
    }
}