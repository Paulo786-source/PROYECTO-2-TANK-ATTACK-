#include "raylib.h"
#include "Renderer.h"
#include "Mapa.h"
#include "Celda.h"

const TipoCelda L = TipoCelda::libre;
const TipoCelda O = TipoCelda::obstaculo;

TipoCelda matriz[FILAS][COLUMNAS] = {
    {L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L},
    {L,O,O,L,L,L,L,L,L,L,L,L,L,O,O,L,L,L,L,L,L,L,L,L,O,O,L,L,L,L,L,L,L,L,L,L,L,O,O,L},
    {L,O,L,L,L,L,L,L,L,L,L,L,L,L,O,L,L,L,L,L,L,L,L,L,L,O,L,L,L,L,L,L,L,L,L,L,L,L,O,L},
    {L,L,L,L,L,O,O,L,L,L,L,L,L,L,L,L,L,L,O,O,L,L,L,L,L,L,L,L,L,O,O,L,L,L,L,L,L,L,L,L},
    {L,L,L,L,L,O,L,L,L,L,L,L,L,L,L,L,L,L,O,L,L,L,L,L,L,L,L,L,L,O,L,L,L,L,L,L,L,L,L,L},
    {L,L,L,L,L,L,L,L,O,O,O,L,L,L,L,L,L,L,L,L,L,O,O,O,L,L,L,L,L,L,L,L,O,O,O,L,L,L,L,L},
    {L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L},
    {L,L,O,O,L,L,L,L,L,L,L,L,L,L,L,O,O,L,L,L,L,L,L,L,L,L,O,O,L,L,L,L,L,L,L,O,O,L,L,L},
    {L,L,O,L,L,L,L,L,L,L,L,L,L,L,L,L,O,L,L,L,L,L,L,L,L,L,L,O,L,L,L,L,L,L,L,L,O,L,L,L},
    {L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L},
    {L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L},
    {L,L,O,L,L,L,L,L,L,L,L,L,L,L,L,L,O,L,L,L,L,L,L,L,L,L,L,O,L,L,L,L,L,L,L,L,O,L,L,L},
    {L,L,O,O,L,L,L,L,L,L,L,L,L,L,L,O,O,L,L,L,L,L,L,L,L,L,O,O,L,L,L,L,L,L,L,O,O,L,L,L},
    {L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L},
    {L,L,L,L,L,L,L,L,O,O,O,L,L,L,L,L,L,L,L,L,L,O,O,O,L,L,L,L,L,L,L,L,O,O,O,L,L,L,L,L},
    {L,L,L,L,L,O,L,L,L,L,L,L,L,O,L,L,L,L,L,L,L,L,L,L,L,O,L,L,L,L,L,L,L,L,L,L,L,L,L,L},
    {L,L,L,L,L,O,O,L,L,L,L,L,O,O,L,L,L,L,O,O,L,L,L,L,L,O,O,L,L,L,L,L,O,O,L,L,L,L,L,L},
    {L,O,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,O,L},
    {L,O,O,L,L,L,L,L,L,L,L,L,L,L,L,L,L,O,O,L,L,L,L,L,L,L,L,L,L,L,L,O,O,L,L,L,L,O,O,L},
    {L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L},
    {L,L,O,O,L,L,L,L,L,O,O,L,L,L,L,L,L,L,L,L,O,O,L,L,L,L,L,L,O,O,L,L,L,L,L,L,L,L,L,L},
    {L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L}
};

int main() { 
    Renderer renderer;
    renderer.inicializar();

    while (renderer.ventanaAbierta()) {
        renderer.iniciarDibujo();
        renderer.dibujarMapaTest(matriz);   
        renderer.terminarDibujo();
    }

    renderer.cerrar();
    return 0;
}