#include "Graph.h"

Graph::Graph(int rows, int cols) : rows(rows), cols(cols) {
    totalNodes = rows * cols;
    adjacencyMatrix = new bool* [totalNodes];
    for (int i = 0; i < totalNodes; ++i) {
        adjacencyMatrix[i] = new bool[totalNodes]();
    }
}

Graph::~Graph() {
    for (int i = 0; i < totalNodes; ++i) {
        delete[] adjacencyMatrix[i];
    }
    delete[] adjacencyMatrix;
}

void Graph::addEdge(int nodeA, int nodeB) {
    adjacencyMatrix[nodeA][nodeB] = true;
    adjacencyMatrix[nodeB][nodeA] = true;
}

void Graph::removeEdge(int nodeA, int nodeB) {
    adjacencyMatrix[nodeA][nodeB] = false;
    adjacencyMatrix[nodeB][nodeA] = false;
}

bool Graph::hasConnection(int nodeA, int nodeB) const {
    return adjacencyMatrix[nodeA][nodeB];
}

void Graph::getNeighbors(int node, int* neighbors, int& count) const {
    for (int i = 0; i < totalNodes; i++) {
        if (adjacencyMatrix[node][i] == true) {
            neighbors[count] = i;
            count++;
        }
    }
}

int Graph::getTotalNodes() const {
    return totalNodes;
}

void Graph::clear() {
    for (int i = 0; i < totalNodes; i++) {
        for (int j = 0; j < totalNodes; j++) {
            adjacencyMatrix[i][j] = false;
        }
    }
}