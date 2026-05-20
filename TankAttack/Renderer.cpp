#include "raylib.h"
#include "Renderer.h"
#include "Map.h"
#include "Cell.h"
#include "Player.h"
#include "Tank.h"


Renderer::Renderer() {}

Renderer::~Renderer() {}

void Renderer::initialize() {
    InitWindow(windowWidth, windowHeight, "Tank Attack!");
    SetTargetFPS(60);

    tankBlue = LoadTexture("textures/tanks/blueTank.png");
    tankCyan = LoadTexture("textures/tanks/lightblueTank.png");
    tankRed = LoadTexture("textures/tanks/redTank.png");
    tankYellow = LoadTexture("textures/tanks/yellowTank.png");
    bullet = LoadTexture("textures/tanks/bullet.png");

    rocks = LoadTexture("textures/map/rocks.png");
    floorPath = LoadTexture("textures/map/dirt.png");
    floor = LoadTexture("textures/map/podzol.png");

    doubleTurn = LoadTexture("textures/hub/doubleTurn.png");
    movePrecision = LoadTexture("textures/hub/movePrecision.png");
    attackPrecision = LoadTexture("textures/hub/attackPrecision.png");
    attackPower = LoadTexture("textures/hub/attackPower.png");

    frame = LoadTexture("textures/hub/frame.png");
    hub = LoadTexture("textures/hub/hub.png");
}

void Renderer::close() {
    UnloadTexture(tankBlue);
    UnloadTexture(tankCyan);
    UnloadTexture(tankRed);
    UnloadTexture(tankYellow);
    UnloadTexture(bullet);
    UnloadTexture(rocks);
    UnloadTexture(floorPath);
    UnloadTexture(floor);
    UnloadTexture(doubleTurn);
    UnloadTexture(movePrecision);
    UnloadTexture(attackPrecision);
    UnloadTexture(attackPower);
    UnloadTexture(frame);
    UnloadTexture(hub);
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
            DrawTexture(floor, col * CELL_SIZE, row * CELL_SIZE, WHITE);
            if (map.getCell(row, col).type == CellType::obstacle) {
                DrawTexture(rocks, col * CELL_SIZE, row * CELL_SIZE, WHITE);
                //DrawRectangle(col * CELL_SIZE, row * CELL_SIZE, CELL_SIZE, CELL_SIZE, GRAY);
            }
        }
    }
}

void Renderer::drawHub() {
    DrawTexture(hub, 0, 704, WHITE);
}

void Renderer::drawTanks(Player& playerOne, Player& playerTwo) {
    for (int p = 0; p < 2; p++) {
        Player& player = (p == 0) ? playerOne : playerTwo;
        for (int i = 0; i < 4; i++) {
            Tank& tank = player.getTank(i);
            if (!tank.isAlive()) continue;

            int x = tank.getPosition().col * CELL_SIZE;
            int y = tank.getPosition().row * CELL_SIZE;

            Texture2D drawTexture;
            switch (tank.getTankColor()) {
            case Tank::TankColor::blue:   drawTexture = tankBlue;   break;
            case Tank::TankColor::cyan:   drawTexture = tankCyan;   break;
            case Tank::TankColor::red:    drawTexture = tankRed;    break;
            case Tank::TankColor::yellow: drawTexture = tankYellow; break;
            }

            Rectangle dest = {
                (float)x + (float)CELL_SIZE / 2,
                (float)y + (float)CELL_SIZE / 2,
                (float)CELL_SIZE,
                (float)CELL_SIZE
            };
            Vector2 origin = { (float)CELL_SIZE / 2, (float)CELL_SIZE / 2 };

            DrawTexturePro(
                drawTexture,
                { 0, 0, (float)drawTexture.width, (float)drawTexture.height },
                dest,
                origin,
                tank.getAngle(),
                WHITE
            );

            if (tank.isSelected()) {
                DrawRectangleLines(x, y, CELL_SIZE, CELL_SIZE, WHITE);
            }
        }
    }
}

void Renderer::drawPath(const Path& path, const Map& map) {
    for (int i = 0; i < path.length; i++) {
        int drawRow;
        int drawCol;
        map.nodeToCoords(path.nodes[i], drawRow, drawCol);
        DrawTexture(floorPath, drawCol * CELL_SIZE, drawRow * CELL_SIZE, WHITE);
        //DrawRectangle(drawCol * CELL_SIZE, drawRow * CELL_SIZE, CELL_SIZE, CELL_SIZE, PURPLE);
    }
}

void Renderer::drawBulletTrail(const BulletPath& shot, const Map& map) {
    for (int i = 0; i < shot.length; i++) {
        int row, col;
        map.nodeToCoords(shot.nodes[i], row, col);
        int x = col * CELL_SIZE + CELL_SIZE / 4;
        int y = row * CELL_SIZE + CELL_SIZE / 4;
        //DrawRectangle(x, y, CELL_SIZE / 2, CELL_SIZE / 2, ORANGE);
        DrawTexture(bullet, x, y, WHITE);
    }
}

void Renderer::drawHUD(Player& player1, Player& player2) {
    // jugador 1: azul y rojo, lado izquierdo
    for (int i = 0; i < 4; i++) {
        Tank& tank = player1.getTank(i);
        const char* colorName;
        switch (tank.getTankColor()) {
        case Tank::TankColor::blue:   colorName = "Azul";     break;
        case Tank::TankColor::red:    colorName = "Rojo";     break;
        case Tank::TankColor::cyan:   colorName = "Celeste";  break;
        case Tank::TankColor::yellow: colorName = "Amarillo"; break;
        default:                      colorName = "?";        break;
        }
        DrawText(TextFormat("%s: %d", colorName, tank.getHealth()), i * 150 + 10, 715, 20, BLACK);
    }
    // jugador 2: celeste y amarillo, lado derecho
    for (int i = 0; i < 4; i++) {
        Tank& tank = player2.getTank(i);
        const char* colorName;
        switch (tank.getTankColor()) {
        case Tank::TankColor::cyan:   colorName = "Celeste";  break;
        case Tank::TankColor::yellow: colorName = "Amarillo"; break;
        case Tank::TankColor::blue:   colorName = "Azul";     break;
        case Tank::TankColor::red:    colorName = "Rojo";     break;
        default:                      colorName = "?";        break;
        }
        DrawText(TextFormat("%s: %d", colorName, tank.getHealth()), i * 165 + 650, 715, 20, BLACK);
    }
}

void Renderer::drawPowerUps(Player& player1, Player& player2) {
    PowerUpQueue player1Queue = player1.getPowerUpQueue();
    int player1QueueCount = player1Queue.getSize();

    for (int i = 0; i < 10; i++) {
        int x = i * 60 + 10;
        DrawTexture(frame, x, 740, WHITE);

        int x2 = 650 + (i * 60);
        DrawTexture(frame, x2, 740, WHITE);
    }

    for (int i = 0; i < player1QueueCount; i++) {
        PowerUp powerUp = player1Queue.getItem(i);
        Texture2D powerUpTexture;
        switch (powerUp.type) {
        case PowerUpType::doubleTurn:      powerUpTexture = doubleTurn;   break;
        case PowerUpType::movePrecision:   powerUpTexture = movePrecision;  break;
        case PowerUpType::attackPrecision: powerUpTexture = attackPrecision;  break;
        case PowerUpType::attackPower:     powerUpTexture = attackPower;     break;
            //default:                           powerUpTexture = WHITE;   break;
        }


        int x = i * 60 + 10;

        DrawTexture(powerUpTexture, x, 742, WHITE);
        //DrawRectangle(x, 755, CELL_SIZE / 2, CELL_SIZE / 2, powerUpColor);

        if (i == 0) {
            DrawRectangleLines(x, 742, CELL_SIZE * 1.5, CELL_SIZE * 1.5, BLACK);
        }
    }

    PowerUpQueue player2Queue = player2.getPowerUpQueue();
    int player2QueueCount = player2Queue.getSize();

    for (int i = 0; i < player2QueueCount; i++) {
        PowerUp powerUp = player2Queue.getItem(i);
        Texture2D powerUpTexture;
        switch (powerUp.type) {
        case PowerUpType::doubleTurn:      powerUpTexture = doubleTurn;   break;
        case PowerUpType::movePrecision:   powerUpTexture = movePrecision;  break;
        case PowerUpType::attackPrecision: powerUpTexture = attackPrecision;  break;
        case PowerUpType::attackPower:     powerUpTexture = attackPower;     break;
            //default:                           powerUpTexture = WHITE;   break;
        }
        int x = 650 + (i * 60);
        DrawTexture(powerUpTexture, x, 742, WHITE);
        //DrawRectangle(x, 755, CELL_SIZE / 2, CELL_SIZE / 2, powerUpColor);

        if (i == 0) {
            DrawRectangleLines(x, 742, CELL_SIZE * 1.5, CELL_SIZE * 1.5, BLACK);
        }
    }
}