#include "Game.h"
#include <cstdlib>

Game::Game() : player1(1), player2(2), activePowerUp({ PowerUpType::doubleTurn }) {
    renderer.initialize();
}

void Game::run() {
    while (renderer.windowOpen()) {
        handleInput();
        update();
        render();
    }
    renderer.close();
}

void Game::handleInput() {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        int clickCol = GetMouseX() / CELL_SIZE;
        int clickRow = GetMouseY() / CELL_SIZE;

        // verificar que el click esté dentro del mapa
        if (clickRow < 0 || clickRow >= ROWS || clickCol < 0 || clickCol >= COLS)
            return;

        Player& currentPlayer = (currentTurn == 1) ? player1 : player2;

        if (selectedTank == nullptr) {
            // buscar si hay un tanque del jugador actual en esa celda
            for (int i = 0; i < 4; i++) {
                Tank& tank = currentPlayer.getTank(i);
                if (tank.isAlive() &&
                    tank.getPosition().row == clickRow &&
                    tank.getPosition().col == clickCol) {
                    tank.setSelected(true);
                    selectedTank = &tank;
                    break;
                }
            }
        }
        else {

            // verificar si clickeó sobre otro tanque del mismo jugador
            bool clickedAnotherTank = false;
            for (int i = 0; i < 4; i++) {
                Tank& tank = currentPlayer.getTank(i);
                if (tank.isAlive() &&
                    tank.getPosition().row == clickRow &&
                    tank.getPosition().col == clickCol) {
                    // deseleccionar el anterior
                    selectedTank->setSelected(false);
                    // seleccionar el nuevo
                    tank.setSelected(true);
                    selectedTank = &tank;
                    clickedAnotherTank = true;
                    break;
                }
            }

            if (!clickedAnotherTank) {
                bool cellOccupied = false;
                for (int i = 0; i < 4; i++) {
                    Tank& t1 = player1.getTank(i);
                    Tank& t2 = player2.getTank(i);
                    if ((t1.isAlive() && t1.getPosition().row == clickRow && t1.getPosition().col == clickCol) ||
                        (t2.isAlive() && t2.getPosition().row == clickRow && t2.getPosition().col == clickCol)) {
                        cellOccupied = true;
                        break;
                    }
                }
                // mover el tanque al destino
                if (map.getCell(clickRow, clickCol).type == CellType::free && !cellOccupied) {
                    int rowTank = selectedTank->getPosition().row;
                    int colTank = selectedTank->getPosition().col;
                    Position origin = { rowTank, colTank };
                    Position destination = { clickRow, clickCol };

                    delete currentBullet;
                    currentBullet = nullptr;

                    int bfsProb = 50;
                    int dijkstraProb = 80;

                    // si hay power-up de precisión activo, aumenta la probabilidad al 90%
                    if (hasPendingPowerUp && activePowerUp.type == PowerUpType::movePrecision) {
                        bfsProb = 90;
                        dijkstraProb = 90;
                    }

                    hasPendingPowerUp = false;

                    // marcar celdas ocupadas por tanques
                    bool blocked[ROWS * COLS] = {};
                    for (int p = 0; p < 2; p++) {
                        Player& pl = (p == 0) ? player1 : player2;
                        for (int t = 0; t < 4; t++) {
                            Tank& tank = pl.getTank(t);
                            if (tank.isAlive()) {
                                int node = map.coordsToNode(tank.getPosition().row, tank.getPosition().col);
                                blocked[node] = true;
                            }
                        }
                    }
                    // el tanque que se mueve no se bloquea a sí mismo
                    blocked[map.coordsToNode(origin.row, origin.col)] = false;

                    currentPath = Path();
                    currentPath = Pathfinding::calculatePath(map, origin, destination,
                        selectedTank->getTankType() == Tank::TankType::BFS, blocked, bfsProb, dijkstraProb);

                    if (currentPath.length > 0) {
                        int lastNode = currentPath.nodes[currentPath.length - 1];
                        int destRow;
                        int destCol;
                        map.nodeToCoords(lastNode, destRow, destCol);
                        selectedTank->setPosition({ destRow, destCol });
                    }


                    selectedTank->setSelected(false);
                    selectedTank = nullptr;
                    nextTurn();
                }

            }
        }
    }

    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
        if (selectedTank == nullptr) return;

        int clickCol = GetMouseX() / CELL_SIZE;
        int clickRow = GetMouseY() / CELL_SIZE;

        if (clickRow < 0 || clickRow >= ROWS || clickCol < 0 || clickCol >= COLS) return;

        Position origin = selectedTank->getPosition();
        Position target = { clickRow, clickCol };

        currentPath = Path();

        delete currentBullet;
        currentBullet = nullptr;

        BulletPath trail;

        // para precisión de ataque la bala sigue A* hacia el objetivo
        if (hasPendingPowerUp && activePowerUp.type == PowerUpType::attackPrecision) {
            trail = Pathfinding::calculateprecisionShot(map, origin, target, player1, player2);
        }
        else {
            trail = Pathfinding::calculateBulletPath(map, origin, target, player1, player2);
        }

        if (hasPendingPowerUp && activePowerUp.type == PowerUpType::attackPower) {
            currentBullet = new Bullet(trail, selectedTank, true);
            dealBulletDamage(*currentBullet);
        }
        else {
            currentBullet = new Bullet(trail, selectedTank, false);
            dealBulletDamage(*currentBullet);
        }

        hasPendingPowerUp = false;

        selectedTank->setSelected(false);
        selectedTank = nullptr;
        currentTurn = (currentTurn == 1) ? 2 : 1;
    }

    if (IsKeyPressed(KEY_LEFT_SHIFT)) {
        Player& currentPlayer = (currentTurn == 1) ? player1 : player2;
        if (currentPlayer.hasPowerUps()) {
            activePowerUp = currentPlayer.usePowerUp();
            switch (activePowerUp.type) {
            case PowerUpType::doubleTurn:      powerUpMessage = "Power-up: Doble Turno";           break;
            case PowerUpType::movePrecision:   powerUpMessage = "Power-up: Precision de Movimiento"; break;
            case PowerUpType::attackPower:     powerUpMessage = "Power-up: Poder de Ataque";        break;
            default:                           powerUpMessage = "";                                  break;
            case PowerUpType::attackPrecision: powerUpMessage = "Power-up: Precision de Ataque"; break;
            }
            if (activePowerUp.type == PowerUpType::doubleTurn) {
                extraTurns = 2;
                hasPendingPowerUp = false;
            }
            nextTurn();
        }
    }
}

void Game::dealBulletDamage(const Bullet& bullet) {
    const BulletPath& shot = bullet.getBulletPath();

    if (!shot.hitTank) return;

    int row, col;
    map.nodeToCoords(shot.hitTankNode, row, col);

    for (int p = 0; p < 2; p++) {
        Player& pl = (p == 0) ? player1 : player2;
        for (int i = 0; i < 4; i++) {
            Tank& t = pl.getTank(i);
            if (!t.isAlive()) continue;
            if (t.getPosition().row != row || t.getPosition().col != col) continue;

            
            // blue/cyan reciben menos daño que red/yellow
            Tank::TankColor c = t.getTankColor();
            if (bullet.isFullPower()) {
                t.receiveDamage(100);
            }
            else {
                if (c == Tank::TankColor::blue || c == Tank::TankColor::cyan)
                    t.receiveDamage(25);
                else
                    t.receiveDamage(50);
            }

        }
    }
}

void Game::update() {
    // pendiente para fases siguientes
}

void Game::render() {
    renderer.beginFrame();
    renderer.drawMap(map);
    renderer.drawPath(currentPath, map);
    renderer.drawTanks(player1, player2);
    if (currentBullet != nullptr) {
        renderer.drawBulletTrail(currentBullet->getBulletPath(), map);
    }
    
    if (currentTurn == 1) {
        DrawText("Turno: Jugador 1", 10, 10, 20, BLUE);
    }
    else {
        DrawText("Turno: Jugador 2", 10, 10, 20, RED);
    }
    renderer.drawHUD(player1, player2);
    renderer.drawPowerUps(player1, player2);
    DrawText(powerUpMessage, 10, 778, 16, BLACK);
    renderer.endFrame();
}

void Game::nextTurn() {
    if (extraTurns > 0) {
        extraTurns--;
    }
    else {
        currentTurn = (currentTurn == 1) ? 2 : 1;
    }
    randomPowerUp();
}

void Game::randomPowerUp() {
    for (int p = 1; p <= 2; p++) {
        Player& player = (p == 1) ? player1 : player2;

        if (rand() % 4 != 0) continue; // 25% por jugador por turno

        // no acumular muchos
        if (player.getPowerUpCount() >= MAX_POWERUPS) continue;

        PowerUpType type = (PowerUpType)(rand() % 4);
        player.addPowerUp({ type });
    }
}