#include "raylib.h"
#include "Renderer.h"
#include "Map.h"
#include "Cell.h"
#include "Player.h"
#include "Tank.h"


Renderer::Renderer() : windowWidth(1280), windowHeight(800) {}

Renderer::~Renderer() {}

void Renderer::initialize() {
    InitWindow(windowWidth, windowHeight, "Tank Attack!");
    SetTargetFPS(60);

    tankBlue = LoadTexture("textures/tanks/blueTank.png");
    tankCyan = LoadTexture("textures/tanks/lightblueTank.png");
    tankRed = LoadTexture("textures/tanks/redTank.png");
    tankYellow = LoadTexture("textures/tanks/yellowTank.png");
    bullet = LoadTexture("textures/tanks/bullet.png");
}

void Renderer::close() {
    UnloadTexture(tankBlue);
    UnloadTexture(tankCyan);
    UnloadTexture(tankRed);
    UnloadTexture(tankYellow);
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
            Texture2D drawTexture;
            switch (color) {
            case Tank::TankColor::blue:   drawTexture = tankBlue;     break;
            case Tank::TankColor::cyan:   drawTexture = tankCyan;  break;
            case Tank::TankColor::red:    drawTexture = tankRed;      break;
            case Tank::TankColor::yellow: drawTexture = tankYellow;   break;
            }
            Rectangle dest = {
                (float)x + (float)CELL_SIZE / 2,
                (float)y + (float)CELL_SIZE / 2,
                (float)CELL_SIZE,
                (float)CELL_SIZE
            };
            Vector2 origin = { (float)CELL_SIZE / 2, (float)CELL_SIZE / 2 };
            float angle = tank.getAngle();
            DrawTexturePro(
                drawTexture,                                                         // la textura
                { 0, 0, (float)drawTexture.width, (float)drawTexture.height },       // rectángulo fuente: { 0, 0, ancho, alto de la imagen }
                dest,                                                                // rectángulo destino: { x, y, CELL_SIZE, CELL_SIZE }
                origin,                                                              // punto de rotación: { CELL_SIZE/2, CELL_SIZE/2 }
                angle,                                                               // ángulo de rotación
                WHITE                                                                // tinte de color
            );
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
            Texture2D drawTexture;
            switch (color2) {
            case Tank::TankColor::blue:   drawTexture = tankBlue;     break;
            case Tank::TankColor::cyan:   drawTexture = tankCyan;  break;
            case Tank::TankColor::red:    drawTexture = tankRed;      break;
            case Tank::TankColor::yellow: drawTexture = tankYellow;   break;
            }
            Rectangle dest = {
                (float)x2 + (float)CELL_SIZE / 2,
                (float)y2 + (float)CELL_SIZE / 2,
                (float)CELL_SIZE,
                (float)CELL_SIZE
            };
            Vector2 origin = { (float)CELL_SIZE / 2, (float)CELL_SIZE / 2 };
            float angle = tank2.getAngle();
            DrawTexturePro(
                drawTexture,                                                         // la textura
                { 0, 0, (float)drawTexture.width, (float)drawTexture.height },       // rectángulo fuente: { 0, 0, ancho, alto de la imagen }
                dest,                                                                // rectángulo destino: { x, y, CELL_SIZE, CELL_SIZE }
                origin,                                                              // punto de rotación: { CELL_SIZE/2, CELL_SIZE/2 }
                angle,                                                               // ángulo de rotación
                WHITE                                                                // tinte de color
            );
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

void Renderer::drawBulletTrail(const BulletPath& shot, const Map& map) {
    for (int i = 0; i < shot.length; i++) {
        int row, col;
        map.nodeToCoords(shot.nodes[i], row, col);
        int x = col * CELL_SIZE + CELL_SIZE / 4;
        int y = row * CELL_SIZE + CELL_SIZE / 4;
        //DrawRectangle(x, y, CELL_SIZE / 2, CELL_SIZE / 2, ORANGE);
        Texture2D drawBullet = bullet;
        DrawTexture(drawBullet, x, y, WHITE);
    }
}

void Renderer::drawHUD(Player& player1, Player& player2) {
    for (int i = 0; i < 4; i++) {
        Tank& tank = player1.getTank(i);
        int health = tank.getHealth();
        const char* colorName;
        switch (tank.getTankColor()) {
            case Tank::TankColor::blue:   colorName = "Azul";    break;
            case Tank::TankColor::red:    colorName = "Rojo";    break;
            default:                      colorName = "?";       break;
        }
        int x = i * 150 + 10;
        DrawText(TextFormat("%s: %d", colorName, health), x, 715, 20, BLACK);
    }
    for (int i = 0; i < 4; i++) {
        Tank& tank2 = player2.getTank(i);
        int health2 = tank2.getHealth();
        const char* colorName2;
        switch (tank2.getTankColor()) {
            case Tank::TankColor::cyan:   colorName2 = "Celeste";    break;
            case Tank::TankColor::yellow:    colorName2 = "Amarillo";    break;
            default:                      colorName2 = "?";       break;
        }
        int x = i * 165 + 650;
        DrawText(TextFormat("%s: %d", colorName2, health2), x, 715, 20, BLACK);
    }
}

void Renderer::drawPowerUps(Player& player1, Player& player2) {
    PowerUpQueue player1Queue = player1.getPowerUpQueue();
    int player1QueueCount = player1Queue.getSize();

    for (int i = 0; i < player1QueueCount; i++) {
        PowerUp powerUp = player1Queue.getItem(i);
        Color powerUpColor;
        switch (powerUp.type) {
            case PowerUpType::doubleTurn:      powerUpColor = GREEN;   break;
            case PowerUpType::movePrecision:   powerUpColor = ORANGE;  break;
            case PowerUpType::attackPrecision: powerUpColor = PURPLE;  break;
            case PowerUpType::attackPower:     powerUpColor = RED;     break;
            default:                           powerUpColor = WHITE;   break;
        }
        int x = i * 20 + 10;
        DrawRectangle(x, 755, CELL_SIZE / 2, CELL_SIZE / 2, powerUpColor);

        if (i == 0) {
            DrawRectangleLines(x, 755, CELL_SIZE / 2, CELL_SIZE / 2, BLACK);
        }
    }

    PowerUpQueue player2Queue = player2.getPowerUpQueue();
    int player2QueueCount = player2Queue.getSize();

    for (int i = 0; i < player2QueueCount; i++) {
        PowerUp powerUp = player2Queue.getItem(i);
        Color powerUpColor;
        switch (powerUp.type) {
        case PowerUpType::doubleTurn:      powerUpColor = GREEN;   break;
        case PowerUpType::movePrecision:   powerUpColor = ORANGE;  break;
            //case PowerUpType::attackPrecision: powerUpColor = PURPLE;  break;
        case PowerUpType::attackPower:     powerUpColor = RED;     break;
        default:                           powerUpColor = WHITE;   break;
        }
        int x = 650 + (i * 20 + 10);
        DrawRectangle(x, 755, CELL_SIZE / 2, CELL_SIZE / 2, powerUpColor);

        if (i == 0) {
            DrawRectangleLines(x, 755, CELL_SIZE / 2, CELL_SIZE / 2, BLACK);
        }
    }
}