#include "raylib.h"
#include "Renderer.h"
#include "Map.h"
#include "Cell.h"
#include "Player.h"
#include "Tank.h"


Renderer::Renderer() : windowWidth(1280), windowHeight(720) {}

Renderer::~Renderer() {}

void Renderer::initialize() {
    InitWindow(windowWidth, windowHeight, "Tank Attack!");
    SetTargetFPS(60);
}

void Renderer::close() {
    CloseWindow();
}

bool Renderer::windowOpen() const {
    return !WindowShouldClose();
}

void Renderer::beginFrame() {
    BeginDrawing();
    ClearBackground(RAYWHITE);
}

void Renderer::endFrame() {
    EndDrawing();
}

void Renderer::drawMap(const Map& map) {
    for (int row = 0; row < map.getRows(); ++row) {
        for (int col = 0; col < map.getCols(); ++col) {
            if (map.getCell(row, col).type == CellType::obstacle) {
                DrawRectangle(col * CELL_SIZE, row * CELL_SIZE, CELL_SIZE, CELL_SIZE, GRAY);
            }
            else {
                DrawRectangle(col * CELL_SIZE, row * CELL_SIZE, CELL_SIZE, CELL_SIZE, LIGHTGRAY);
            }
        }
    }
}

void Renderer::drawTanks(Player& playerOne, Player& playerTwo) {
    for (int i = 0; i < 4; i++) {
        // Jugador 1
        Tank& tank = playerOne.getTank(i);
        if (tank.isAlive()) {
            int x = tank.getPosition().col * CELL_SIZE;
            int y = tank.getPosition().row * CELL_SIZE;
            bool selected = tank.isSelected();
            Tank::TankColor color = tank.getTankColor();
            Color drawColor;
            switch (color) {
                case Tank::TankColor::blue:   drawColor = BLUE;     break;
                case Tank::TankColor::cyan:   drawColor = SKYBLUE;  break;
                case Tank::TankColor::red:    drawColor = RED;      break;
                case Tank::TankColor::yellow: drawColor = YELLOW;   break;
            }
            DrawRectangle(x, y, CELL_SIZE, CELL_SIZE, drawColor);
            if (selected) {
                DrawRectangleLines(x, y, CELL_SIZE, CELL_SIZE, WHITE);
            }
        }


    }
    for (int j = 0; j < 4; j++) {
        // Jugador 2
        Tank& tank2 = playerTwo.getTank(j);
        if (tank2.isAlive()) {
            int x2 = tank2.getPosition().col * CELL_SIZE;
            int y2 = tank2.getPosition().row * CELL_SIZE;
            bool selected2 = tank2.isSelected();
            Tank::TankColor color2 = tank2.getTankColor();
            Color drawColor2;
            switch (color2) {
                case Tank::TankColor::blue:   drawColor2 = BLUE;     break;
                case Tank::TankColor::cyan:   drawColor2 = SKYBLUE;  break;
                case Tank::TankColor::red:    drawColor2 = RED;      break;
                case Tank::TankColor::yellow: drawColor2 = YELLOW;   break;
            }
            DrawRectangle(x2, y2, CELL_SIZE, CELL_SIZE, drawColor2);
            if (selected2) {
                DrawRectangleLines(x2, y2, CELL_SIZE, CELL_SIZE, WHITE);
            }
        }

    }
}

void Renderer::drawPath(const Path& path, const Map& map) {
    for (int i = 0; i < path.length; i++) {
        int drawRow;
        int drawCol;
        map.nodeToCoords(path.nodes[i], drawRow, drawCol);
        DrawRectangle(drawCol * CELL_SIZE, drawRow * CELL_SIZE, CELL_SIZE, CELL_SIZE, PURPLE);
    }
}