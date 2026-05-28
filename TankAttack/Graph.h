#pragma once

class Graph {
public:
    Graph(int rows, int cols);
    ~Graph();

    void addEdge(int nodeA, int nodeB);
    void removeEdge(int nodeA, int nodeB);
    bool hasConnection(int nodeA, int nodeB) const;
    void getNeighbors(int node, int* neighbors, int& count) const;
    int getTotalNodes() const;
    void clear();

private:
    int rows;
    int cols;
    int totalNodes;
    bool** adjacencyMatrix;
};