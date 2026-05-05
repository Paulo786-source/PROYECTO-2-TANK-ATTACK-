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
	bool visitado[FILAS * COLUMNAS] = {};

	int cola[FILAS * COLUMNAS];
	int frente = 0;
	int fin = 0;

	bool encontrado = false;

	for (int i = 0; i < FILAS && !encontrado; i++) {
		for (int j = 0; j < COLUMNAS && !encontrado; j++) {
			if (celdas[i][j].tipo == TipoCelda::libre) {
				int nodo = coordsANodo(i, j);
				visitado[nodo] = true;
				cola[fin] = nodo;
				fin++;
				encontrado = true;
			}
		}
	}

	while (frente != fin) {
		int nodo = cola[frente];
		frente++;
		int vecinos[4];
		int cantidad = 0;
		grafo->obtenerVecinos(nodo, vecinos, cantidad);

		for (int i = 0; i < cantidad; i++) {
			int vecino = vecinos[i];
			if (visitado[vecino] == false) {
				visitado[vecino] = true;
				cola[fin] = vecino;
				fin++;
			}
		}
	}

	for (int i = 0; i < FILAS; i++) {
		for (int j = 0; j < COLUMNAS; j++) {
			int nodo = coordsANodo(i, j);
			if (celdas[i][j].tipo == TipoCelda::libre && visitado[nodo] == false) {
				return false;
			}
		}
	}
	return true;
}
