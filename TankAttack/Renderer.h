#pragma once
#include "raylib.h"
#include "Map.h"

const int CELL_SIZE = 32;

class Renderer {
public:
    Renderer();
    ~Renderer();

    void initialize();
    void close();
    bool windowOpen() const;
    void beginFrame();
    void endFrame();

    void drawMap(const Map& map);

    // las siguientes funciones se implementan en las siguientes fases

    // void drawTanks();
    // void drawPath();
    // void drawBullets();
    // void drawHUD();
    // void drawPowerUps();

private:
    int windowWidth;
    int windowHeight;
};