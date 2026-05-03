#pragma once
#include "Celda.h"
#include "Grafo.h"

const int FILAS = 22;
const int COLUMNAS = 40;

class Mapa {
public:
    Mapa();
    ~Mapa();

    // genera el mapa con obstaculos aleatorios y verifica conectividad total
    void generarAleatorio();

    Celda& obtenerCelda(int fila, int columna);
    const Celda& obtenerCelda(int fila, int columna) const;

    // retorna vecinos accesibles de una celda para pathfinding
    void getNeighbors(int fila, int columna, int* vecinos, int& cantidad) const;

    int coordsANodo(int fila, int columna) const;
    void nodoEnCoordenadas(int nodo, int& fila, int& columna) const;

    Grafo* obtenerGrafo();
    int obtenerFilas() const;
    int obtenerColumnas() const;

private:
    Celda celdas[FILAS][COLUMNAS];
    Grafo* grafo;

    bool verificarConectividad() const;
};