#pragma once
#include "Cell.h"
#include "Graph.h"

const int ROWS = 22;
const int COLS = 40;

class Map {
public:
    Map();
    ~Map();

    // genera el mapa con obstaculos aleatorios y verifica conectividad total
    void generateRandom();

    Cell& getCell(int row, int col);
    const Cell& getCell(int row, int col) const;

    // retorna vecinos accesibles de una celda para pathfinding
    void getNeighbors(int row, int col, int* neighbors, int& count) const;

    int coordsToNode(int row, int col) const;
    void nodeToCoords(int node, int& row, int& col) const;

    Graph* getGraph() const;
    int getRows() const;
    int getCols() const;

private:
    Cell cells[ROWS][COLS];
    Graph* graph;

    bool checkConnectivity() const;
};