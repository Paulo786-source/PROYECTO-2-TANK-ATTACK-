#include "raylib.h"
#include "Renderer.h"
#include "Map.h"
#include "Cell.h"

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