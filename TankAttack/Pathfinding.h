#pragma once
#include "Mapa.h"

struct Posicion {
    int fila;
    int columna;
};

struct Ruta {
    int nodos[FILAS * COLUMNAS];
    int longitud;
    Ruta() : longitud(0) {}
};

// radio en celdas para el movimiento aleatorio 
const int RADIO_ALEATORIO = 3;

class Pathfinding {
public:
    // tanques celeste/azul : 50 % BFS, 50 % aleatorio
    // tanques rojo / amarillo : 80 % Dijkstra, 20 % aleatorio
    static Ruta calcularRuta(const Mapa& mapa, Posicion origen, Posicion destino, bool esBFS);

private:
    static Ruta avanzarHastaObstaculo(const Mapa& mapa, Posicion origen, Posicion destino);
    static Ruta BFS(const Mapa& mapa, Posicion origen, Posicion destino);
    static Ruta Dijkstra(const Mapa& mapa, Posicion origen, Posicion destino);
    static Ruta movimientoAleatorio(const Mapa& mapa, Posicion origen, Posicion destino);

    static Ruta reconstruirRuta(const Mapa& mapa, int* padre, int nodoOrigen, int nodoDestino);
    static bool hayLineaVista(const Mapa& mapa, Posicion a, Posicion b);
    static void agregarSegmento(const Mapa& mapa, Ruta& ruta, Posicion desde, Posicion hasta, bool saltarPrimero);
};