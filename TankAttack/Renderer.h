#pragma once
#include "raylib.h"
#include "Map.h"
#include "Player.h"

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
    void drawBulletTrail(const BulletPath& shot, const Map& map);

   
    // las siguientes funciones se implementan en las siguientes fases

    void drawTanks(Player& one, Player& two);
    void drawPath(const Path& path, const Map& map);
    // void drawBullets();
    void drawHUD(Player& player1, Player& player2);
    void drawPowerUps(Player& player1, Player& player2);

private:
    int windowWidth;
    int windowHeight;

    Texture2D tankBlue;
    Texture2D tankCyan;
    Texture2D tankRed;
    Texture2D tankYellow;
    Texture2D bullet;

    Texture2D rocks;
    Texture2D floor;
    Texture2D floorPath;

    Texture2D doubleTurn;
    Texture2D movePrecision;
    Texture2D attackPrecision;
    Texture2D attackPower;
};