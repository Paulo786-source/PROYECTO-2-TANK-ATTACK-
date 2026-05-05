#include "Grafo.h"

Grafo::Grafo(int filas, int columnas) : filas(filas), columnas(columnas) {
	totalNodos = filas * columnas;
	matrizAdyacencia = new bool*[totalNodos];
	for (int i = 0; i < totalNodos; ++i) {
		matrizAdyacencia[i] = new bool[totalNodos]();
	}
}

Grafo::~Grafo(){
	for (int i = 0; i < totalNodos; ++i) {
		delete[] matrizAdyacencia[i];
	}
	delete[] matrizAdyacencia;
}

void Grafo::agregarArista(int nodoA, int nodoB){
	matrizAdyacencia[nodoA][nodoB] = true;
	matrizAdyacencia[nodoB][nodoA] = true;
}

void Grafo::eliminarArista(int nodoA, int nodoB) {
	matrizAdyacencia[nodoA][nodoB] = false;
	matrizAdyacencia[nodoB][nodoA] = false;
}

bool Grafo::hayConexion(int nodoA, int nodoB) const {
	return matrizAdyacencia[nodoA][nodoB];
}

void Grafo::obtenerVecinos(int nodo, int* vecinos, int& cantidad) const {
	for (int i = 0; i < totalNodos; i++) {
		if (matrizAdyacencia[nodo][i] == true) {
			vecinos[cantidad] = i;
			cantidad++;
		}
	}
}

int Grafo::obtenerTotalNodos() const {
	return totalNodos;
}