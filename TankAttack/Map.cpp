#include "Map.h"
#include "Graph.h"
#include <cstdlib>

Map::Map() {
    graph = new Graph(ROWS, COLS);
    generateRandom();
}

Map::~Map() {
    delete graph;
}

// Version 1 - no const
Cell& Map::getCell(int row, int col) {
    return cells[row][col];
}
// Version 2 - const
const Cell& Map::getCell(int row, int col) const {
    return cells[row][col];
}

int Map::coordsToNode(int row, int col) const {
    return row * COLS + col;
}

void Map::nodeToCoords(int node, int& row, int& col) const {
    row = node / COLS;
    col = node % COLS;
}

int Map::getRows() const {
    return ROWS;
}

int Map::getCols() const {
    return COLS;
}

Graph* Map::getGraph() const {
    return graph;
}

void Map::getNeighbors(int row, int col, int* neighbors, int& count) const {
    int node = coordsToNode(row, col);
    graph->getNeighbors(node, neighbors, count);
}

bool Map::checkConnectivity() const {
    bool visited[ROWS * COLS] = {};
    int queue[ROWS * COLS];
    int front = 0;
    int back = 0;
    bool found = false;

    for (int i = 0; i < ROWS && !found; i++) {
        for (int j = 0; j < COLS && !found; j++) {
            if (cells[i][j].type == CellType::free) {
                int node = coordsToNode(i, j);
                visited[node] = true;
                queue[back] = node;
                back++;
                found = true;
            }
        }
    }

    while (front != back) {
        int node = queue[front];
        front++;
        int neighbors[4];
        int count = 0;
        graph->getNeighbors(node, neighbors, count);

        for (int i = 0; i < count; i++) {
            int neighbor = neighbors[i];
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                queue[back] = neighbor;
                back++;
            }
        }
    }

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            int node = coordsToNode(i, j);
            if (cells[i][j].type == CellType::free && !visited[node]) {
                return false;
            }
        }
    }
    return true;
}

void Map::generateRandom() {
    bool connected = false;
    while (!connected) {
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                cells[i][j].type = CellType::free;
            }
        }

        graph->clear();

        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                int random = rand() % 100;
                bool topLeft     = (i <= 1 && j <= 1);
                bool topRight    = (i <= 1 && j >= COLS - 2);
                bool bottomLeft  = (i >= ROWS - 2 && j <= 1);
                bool bottomRight = (i >= ROWS - 2 && j >= COLS - 2);

                if (random < 20 && !topLeft && !topRight && !bottomLeft && !bottomRight) {
                    cells[i][j].type = CellType::obstacle;
                }
            }
        }

        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                if (cells[i][j].type == CellType::free) {
                    int mainNode = coordsToNode(i, j);

                    // vecino arriba
                    if (i > 0 && cells[i - 1][j].type == CellType::free)
                        graph->addEdge(mainNode, coordsToNode(i - 1, j));

                    // vecino abajo
                    if (i < ROWS - 1 && cells[i + 1][j].type == CellType::free)
                        graph->addEdge(mainNode, coordsToNode(i + 1, j));

                    // vecino izquierda
                    if (j > 0 && cells[i][j - 1].type == CellType::free)
                        graph->addEdge(mainNode, coordsToNode(i, j - 1));

                    // vecino derecha
                    if (j < COLS - 1 && cells[i][j + 1].type == CellType::free)
                        graph->addEdge(mainNode, coordsToNode(i, j + 1));
                }
            }
        }

        connected = checkConnectivity();
    }
}