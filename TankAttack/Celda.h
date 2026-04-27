#pragma once

enum class TipoCelda {
    libre,
    obstaculo
};

struct Celda {
    TipoCelda tipo;
    bool tieneTanque;
    bool tieneBala;

    Celda() : tipo(TipoCelda::libre), tieneTanque(false), tieneBala(false) {}
};