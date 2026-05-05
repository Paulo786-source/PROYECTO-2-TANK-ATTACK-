#include "mapa.h"
#include "Grafo.h"
#include <cstdlib>


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

void Mapa::generarAleatorio() {
	bool conectado = false;
	while (!conectado){
		for (int i = 0; i < FILAS; i++) {
			for (int j = 0; j < COLUMNAS; j++) {
				celdas[i][j].tipo = TipoCelda::libre;
			}
		}

		grafo->limpiar();



		for (int i = 0; i < FILAS; i++) {
			for (int j = 0; j < COLUMNAS; j++) {
				int random = rand() % 100;
				bool esquinaSuperiorIzq = (i <= 1 && j <= 1);
				bool esquinaSuperiorDer = (i <= 1 && j >= COLUMNAS - 2);
				bool esquinaInferiorIzq = (i >= FILAS - 2 && j <= 1);
				bool esquinaInferiorDer = (i >= FILAS - 2 && j >= COLUMNAS - 2);

				if (random < 20 && !esquinaSuperiorIzq && !esquinaSuperiorDer && !esquinaInferiorIzq && !esquinaInferiorDer) {
					celdas[i][j].tipo = TipoCelda::obstaculo;
				}
			}
		}



		for (int i = 0; i < FILAS; i++) {
			for (int j = 0; j < COLUMNAS; j++) {
				if (celdas[i][j].tipo == TipoCelda::libre) {
					int nodoPrincipal = coordsANodo(i, j);

					// vecino arriba
					if (i > 0 && celdas[i - 1][j].tipo == TipoCelda::libre) {
						grafo->agregarArista(nodoPrincipal, coordsANodo(i - 1, j));
					}

					// vecino abajo
					if (i < FILAS - 1 && celdas[i + 1][j].tipo == TipoCelda::libre) {
						grafo->agregarArista(nodoPrincipal, coordsANodo(i + 1, j));
					}

					// vecino izquierda
					if (j > 0 && celdas[i][j - 1].tipo == TipoCelda::libre) {
						grafo->agregarArista(nodoPrincipal, coordsANodo(i, j - 1));
					}

					// vecino derecha
					if (j < COLUMNAS - 1 && celdas[i][j + 1].tipo == TipoCelda::libre) {
						grafo->agregarArista(nodoPrincipal, coordsANodo(i, j + 1));
					}
				}
			}
		}

		conectado = verificarConectividad();
	}
}


