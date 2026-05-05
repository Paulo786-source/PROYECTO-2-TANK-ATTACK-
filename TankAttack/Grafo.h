#pragma once

class Grafo {
public:
    Grafo(int filas, int columnas);
    ~Grafo();

    void agregarArista(int nodoA, int nodoB);
    void eliminarArista(int nodoA, int nodoB);
    bool hayConexion(int nodoA, int nodoB) const;
    void obtenerVecinos(int nodo, int* vecinos, int& cantidad) const;
    int obtenerTotalNodos() const;
    void limpiar();

private:
    int filas;
    int columnas;
    int totalNodos;
    bool** matrizAdyacencia;
};