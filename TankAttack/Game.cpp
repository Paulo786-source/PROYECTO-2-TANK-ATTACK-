#include "Game.h"

Game::Game() : player1(1), player2(2) {
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
                    selectedTank->setPosition({ clickRow, clickCol });
                    selectedTank->setSelected(false);
                    selectedTank = nullptr;
                    currentTurn = (currentTurn == 1) ? 2 : 1;
                }

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
    renderer.drawTanks(player1, player2);
    if (currentTurn == 1) {
        DrawText("Turno: Jugador 1", 10, 10, 20, BLUE);
    }
    else {
        DrawText("Turno: Jugador 2", 10, 10, 20, RED);
    }
    renderer.endFrame();
}