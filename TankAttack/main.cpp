#include "raylib.h"
#include <cstdlib>
#include <ctime>
#include "Renderer.h"
#include "Mapa.h"
#include "Celda.h"


int main() { 
    srand(time(nullptr));
    Mapa mapa;
    Renderer renderer;
    renderer.inicializar();

    while (renderer.ventanaAbierta()) {
        renderer.iniciarDibujo();
        renderer.dibujarMapa(mapa);   
        renderer.terminarDibujo();
    }

    renderer.cerrar();
    return 0;
}