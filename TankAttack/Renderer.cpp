#include "raylib.h"
#include "Renderer.h"
#include "Mapa.h"
#include "Celda.h"
#include <iostream>

Renderer::Renderer() : anchoVentana(1280), altoVentana(720) {}

Renderer::~Renderer() {}

void Renderer::inicializar() {
	InitWindow(anchoVentana, altoVentana, "Tank Attack!");
	SetTargetFPS(60);
}

void Renderer::cerrar() {
	CloseWindow();
}

bool Renderer::ventanaAbierta() const {
	return !WindowShouldClose();
}

void Renderer::iniciarDibujo() {
	BeginDrawing();
	ClearBackground(RAYWHITE);
}

void Renderer::terminarDibujo() {
	EndDrawing();
}

void Renderer::dibujarMapaTest(TipoCelda matriz[FILAS][COLUMNAS]) {
	for (int fila = 0; fila < FILAS; ++fila) {
		for (int columna = 0; columna < COLUMNAS; ++columna) {
			if (matriz[fila][columna] == TipoCelda::obstaculo) {
				DrawRectangle(columna * TAMANO_CELDA, fila * TAMANO_CELDA, TAMANO_CELDA, TAMANO_CELDA, GRAY);
			}
			else {
				DrawRectangle(columna * TAMANO_CELDA, fila * TAMANO_CELDA, TAMANO_CELDA, TAMANO_CELDA, LIGHTGRAY);
			}
		}
	}
}


//void Renderer::dibujarMapa(const Mapa& mapa) {
//	for (int fila = 0; fila < mapa.obtenerFilas(); ++fila) {
//		for (int columna = 0; columna < mapa.obtenerColumnas(); ++columna) {
//			if (mapa.obtenerCelda(fila, columna).tipo == TipoCelda::obstaculo) {
//				DrawRectangle(columna * TAMANO_CELDA, fila * TAMANO_CELDA, TAMANO_CELDA, TAMANO_CELDA, GRAY);
//			} else {
//				DrawRectangle(columna * TAMANO_CELDA, fila * TAMANO_CELDA, TAMANO_CELDA, TAMANO_CELDA, LIGHTGRAY);
//			}
//		}
//	}
//}
