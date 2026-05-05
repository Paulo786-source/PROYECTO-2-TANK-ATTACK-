#include "mapa.h"
#include "Grafo.h"

Mapa::Mapa() {
	grafo = new Grafo(FILAS, COLUMNAS);
	generarAleatorio();
}
Mapa::~Mapa() {
	delete grafo;
}

// Version 1 - no const
Celda& Mapa::obtenerCelda(int fila, int columna) {
	return celdas[fila][columna];
}

// Version 2 - const
const Celda& Mapa::obtenerCelda(int fila, int columna) const {
	return celdas[fila][columna];
}

int Mapa::coordsANodo(int fila, int columna) const {
	int coordsNodo = fila * COLUMNAS + columna;
	return coordsNodo;
}

void Mapa::nodoEnCoordenadas(int nodo, int& fila, int& columna) const {
	fila = nodo / COLUMNAS;
	columna = nodo % COLUMNAS;
}

int Mapa::obtenerFilas() const {
	return FILAS;
}

int Mapa::obtenerColumnas() const {
	return COLUMNAS;
}

void Mapa::getNeighbors(int fila, int columna, int* vecinos, int& cantidad) const {
	int nodo = coordsANodo(fila, columna);
	grafo->obtenerVecinos(nodo, vecinos, cantidad);
}

bool Mapa::verificarConectividad() const {

}
