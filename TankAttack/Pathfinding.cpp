#include "Pathfinding.h"
#include <cstdlib>
#include <cmath>

Ruta Pathfinding::reconstruirRuta(const Mapa& mapa, int* padre, int nodoOrigen, int nodoDestino) {
    Ruta ruta;

    // recorrer de destino a origen siguiendo los padres
    int actual = nodoDestino;
    while (actual != -1) {
        ruta.nodos[ruta.longitud] = actual;
        ruta.longitud++;
        actual = padre[actual];
    }

    // invertir el arreglo para que quede de origen a destino
    int izquierda = 0;
    int derecha = ruta.longitud - 1;
    while (izquierda < derecha) {
        int temp = ruta.nodos[izquierda];
        ruta.nodos[izquierda] = ruta.nodos[derecha];
        ruta.nodos[derecha] = temp;
        izquierda++;
        derecha--;
    }

    return ruta;
}

bool Pathfinding::hayLineaVista(const Mapa& mapa, Posicion a, Posicion b) {
    int diferenciaFilas = b.fila - a.fila;
    int diferenciaColumnas = b.columna - a.columna;

    // la cantidad de pasos es el mayor de los dos desplazamientos
    int pasos = abs(diferenciaFilas) > abs(diferenciaColumnas) ? abs(diferenciaFilas) : abs(diferenciaColumnas);
    if (pasos == 0) return true; 

    for (int i = 1; i <= pasos; i++) {
        // interpolar la posición en cada paso para trazar la línea
        // se suma 0.5f para no redondear hacia abajo
        int fila = a.fila + (int)((diferenciaFilas * i) / (float)pasos + 0.5f);
        int columna = a.columna + (int)((diferenciaColumnas * i) / (float)pasos + 0.5f);

        if (mapa.obtenerCelda(fila, columna).tipo == TipoCelda::obstaculo) {
            return false;
        }
    }
    return true;
}

// para tanques azul/celeste
Ruta Pathfinding::BFS(const Mapa& mapa, Posicion origen, Posicion destino) {
    int totalNodos = FILAS * COLUMNAS;
    int nodoOrigen = mapa.coordsANodo(origen.fila, origen.columna);
    int nodoDestino = mapa.coordsANodo(destino.fila, destino.columna);

    bool visitado[FILAS * COLUMNAS] = {};
    int  padre[FILAS * COLUMNAS];
    for (int i = 0; i < totalNodos; i++) padre[i] = -1; 

    int cola[FILAS * COLUMNAS];
    int frente = 0, fin = 0;

    visitado[nodoOrigen] = true;
    cola[fin] = nodoOrigen;
    fin++;

    while (frente != fin) {
        int actual = cola[frente];
        frente++;

        if (actual == nodoDestino) break;

        int vecinos[4];
        int cantidad = 0;
        mapa.obtenerGrafo()->obtenerVecinos(actual, vecinos, cantidad);

        for (int i = 0; i < cantidad; i++) {
            int vecino = vecinos[i];
            if (!visitado[vecino]) {
                visitado[vecino] = true;
                padre[vecino] = actual;
                cola[fin] = vecino;
                fin++;
            }
        }
    }

    if (!visitado[nodoDestino]) return Ruta(); // no hay camino posible

    return reconstruirRuta(mapa, padre, nodoOrigen, nodoDestino);
}

// para tanques rojo/amarillo
Ruta Pathfinding::Dijkstra(const Mapa& mapa, Posicion origen, Posicion destino) {
    int totalNodos = FILAS * COLUMNAS;
    int nodoOrigen = mapa.coordsANodo(origen.fila, origen.columna);
    int nodoDestino = mapa.coordsANodo(destino.fila, destino.columna);

    int  distancia[FILAS * COLUMNAS];
    bool visitado[FILAS * COLUMNAS] = {};
    int  padre[FILAS * COLUMNAS];

    // empiezan todas las distancias en infinito
    for (int i = 0; i < totalNodos; i++) {
        distancia[i] = 999999;
        padre[i] = -1;
    }
    distancia[nodoOrigen] = 0;

    for (int iter = 0; iter < totalNodos; iter++) {
        // buscar el nodo no visitado con menor distancia acumulada
        int u = -1;
        for (int i = 0; i < totalNodos; i++) {
            if (!visitado[i] && (u == -1 || distancia[i] < distancia[u])) {
                u = i;
            }
        }

        if (u == -1 || distancia[u] == 999999) break; // no quedan nodos alcanzables
        if (u == nodoDestino) break;

        visitado[u] = true;

        int vecinos[4];
        int cantidad = 0;
        mapa.obtenerGrafo()->obtenerVecinos(u, vecinos, cantidad);

        for (int i = 0; i < cantidad; i++) {
            int vecino = vecinos[i];
            int nuevaDistancia = distancia[u] + 1; // cada celda tiene peso 1
            if (nuevaDistancia < distancia[vecino]) {
                distancia[vecino] = nuevaDistancia;
                padre[vecino] = u;
            }
        }
    }

    if (distancia[nodoDestino] == 999999) return Ruta(); // no hay camino posible

    return reconstruirRuta(mapa, padre, nodoOrigen, nodoDestino);
}

Ruta Pathfinding::avanzarHastaObstaculo(const Mapa& mapa, Posicion origen, Posicion destino) {
    Ruta ruta;
    int diferenciaFilas = destino.fila - origen.fila;
    int diferenciaColumnas = destino.columna - origen.columna;
    int pasos = abs(diferenciaFilas) > abs(diferenciaColumnas) ? abs(diferenciaFilas) : abs(diferenciaColumnas);

    for (int i = 0; i <= pasos; i++) {
        int fila = origen.fila + (int)((diferenciaFilas * i) / (float)(pasos == 0 ? 1 : pasos) + 0.5f);
        int columna = origen.columna + (int)((diferenciaColumnas * i) / (float)(pasos == 0 ? 1 : pasos) + 0.5f);

        // para si se topa con un obstáculo
        if (mapa.obtenerCelda(fila, columna).tipo == TipoCelda::obstaculo) break;

        ruta.nodos[ruta.longitud] = mapa.coordsANodo(fila, columna);
        ruta.longitud++;
    }
    return ruta;
}


// agrega las celdas de una línea recta entre dos posiciones a una ruta existente
void Pathfinding::agregarSegmento(const Mapa& mapa, Ruta& ruta, Posicion desde, Posicion hasta, bool saltarPrimero) {
    int diferenciaFilas = hasta.fila - desde.fila;
    int diferenciaColumnas = hasta.columna - desde.columna;
    int pasos = abs(diferenciaFilas) > abs(diferenciaColumnas) ? abs(diferenciaFilas) : abs(diferenciaColumnas);

    // si saltarPrimero es true entonces empieza en 1 para no repetir el punto de inicio
    int inicio = saltarPrimero ? 1 : 0;

    for (int i = inicio; i <= pasos; i++) {
        int fila = desde.fila + (int)((diferenciaFilas * i) / (float)(pasos == 0 ? 1 : pasos) + 0.5f);
        int columna = desde.columna + (int)((diferenciaColumnas * i) / (float)(pasos == 0 ? 1 : pasos) + 0.5f);
        
        if (mapa.obtenerCelda(fila, columna).tipo == TipoCelda::obstaculo) break;
        
        ruta.nodos[ruta.longitud] = mapa.coordsANodo(fila, columna);
        ruta.longitud++;
    }
}
Ruta Pathfinding::movimientoAleatorio(const Mapa& mapa, Posicion origen, Posicion destino) {
    // primero línea vista directa al destino
    if (hayLineaVista(mapa, origen, destino)) {
        Ruta ruta;
        agregarSegmento(mapa, ruta, origen, destino, false);
        return ruta;
    }

    // si no hay línea vista entonces se elige una celda libre aleatoria dentro del radio
    Posicion intermedia = origen;

    for (int intento = 0; intento < 10; intento++) {
        int saltoDeFila = (rand() % (2 * RADIO_ALEATORIO + 1)) - RADIO_ALEATORIO;
        int saltoDeColumna = (rand() % (2 * RADIO_ALEATORIO + 1)) - RADIO_ALEATORIO;

        int nuevaFila = origen.fila + saltoDeFila;
        int nuevaColumna = origen.columna + saltoDeColumna;

        bool dentroDelMapa = (nuevaFila >= 0 && nuevaFila < FILAS &&
            nuevaColumna >= 0 && nuevaColumna < COLUMNAS);

        if (dentroDelMapa && mapa.obtenerCelda(nuevaFila, nuevaColumna).tipo == TipoCelda::libre) {
            intermedia = { nuevaFila, nuevaColumna };
            break;
        }
    }
    // si no sirve, se intenta línea vista desde la intermedia al destino
    if (hayLineaVista(mapa, intermedia, destino)) {
        Ruta ruta;
        agregarSegmento(mapa, ruta, origen, intermedia, false);
        agregarSegmento(mapa, ruta, intermedia, destino, true);
        return ruta;
    }

    // si ningún intento funcionó, se avanza hasta donde sea posible
    return avanzarHastaObstaculo(mapa, origen, destino);
}

// decide qué algoritmo usar según el tipo de tanque y un número aleatorio
Ruta Pathfinding::calcularRuta(const Mapa& mapa, Posicion origen, Posicion destino, bool esBFS) {
    int probabilidad = rand() % 100;

    if (esBFS) {
        if (probabilidad < 50) {
            return BFS(mapa, origen, destino);
        }
        else {
            return movimientoAleatorio(mapa, origen, destino);
        }
    }
    else {
        if (probabilidad < 80) {
            return Dijkstra(mapa, origen, destino);
        }
        else {
            return movimientoAleatorio(mapa, origen, destino);
        }
    }
}