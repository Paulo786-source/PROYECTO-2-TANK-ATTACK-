#pragma once
#include "raylib.h"
#include "Mapa.h"

const int TAMANO_CELDA = 32;

class Renderer {
public:
    Renderer();
    ~Renderer();

    void inicializar();
    void cerrar();
    bool ventanaAbierta() const;
    void iniciarDibujo();
    void terminarDibujo();

    void dibujarMapa(const Mapa& mapa);

    // las siguientes funciones se implementan en las siguientes fases
    
    // void dibujarTanques();
    // void dibujarRuta();
    // void dibujarBalas();
    // void dibujarHUD();
    // void dibujarPowerUps();

private:
    int anchoVentana;
    int altoVentana;
};