#include "raylib.h"

int main() { 
    InitWindow(800, 600, "Tank Attack!");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("raylib funciona!", 300, 270, 20, DARKGRAY);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}