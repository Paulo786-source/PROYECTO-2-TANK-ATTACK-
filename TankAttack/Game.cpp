#include "Game.h"
#include "GameCommand.h"
#include <cstdlib>

Game::Game() : player1(1), player2(2) {
    renderer.initialize();
    audio.initialize();

    // Observer con AudioManager 
    listeners[listenerCount++] = &audio;

    notify(GameEvent::BackgroundMusicStart);
}

Game::~Game() {
    delete currentBullet;
    currentBullet = nullptr;
}

void Game::notify(GameEvent event) {
    for (int i = 0; i < listenerCount; i++) {
        listeners[i]->onGameEvent(event);
    }
}

void Game::buildBlockedArray(bool* blocked) const {
    for (int p = 0; p < 2; p++) {
        const Player& pl = (p == 0) ? player1 : player2;
        for (int t = 0; t < 4; t++) {
            const Tank& tank = pl.getTank(t);
            if (tank.isAlive()) {
                blocked[map.coordsToNode(tank.getPosition().row, tank.getPosition().col)] = true;
            }
        }
    }
}

void Game::run() {
    while (renderer.windowOpen()) {
        audio.update();
        handleInput();
        if (!gameOver) {
            update();
        }
        render();
    }
    renderer.close();
}

void Game::handleInput() {
    if (gameOver) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            int mx = GetMouseX(), my = GetMouseY();
            if (mx >= 565 && mx <= 715 && my >= 430 && my <= 480) {
                notify(GameEvent::ButtonClick);
                renderer.close();
            }
        }
        return;
    }

    // click izquierdo: seleccionar tanque o mover
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        int clickCol = GetMouseX() / CELL_SIZE;
        int clickRow = GetMouseY() / CELL_SIZE;

        if (clickRow < 0 || clickRow >= ROWS || clickCol < 0 || clickCol >= COLS)
            return;

        Player& currentPlayer = (currentTurn == 1) ? player1 : player2;

        if (selectedTank == nullptr) {
            // seleccionar un tanque del jugador actual
            for (int i = 0; i < 4; i++) {
                Tank& tank = currentPlayer.getTank(i);
                if (tank.isAlive() &&
                    tank.getPosition().row == clickRow &&
                    tank.getPosition().col == clickCol) {
                    tank.setSelected(true);
                    notify(GameEvent::TankSelect);
                    selectedTank = &tank;
                    break;
                }
            }
        }
        else {
            // intentar cambiar selección a otro tanque del mismo jugador
            bool clickedAnotherTank = false;
            for (int i = 0; i < 4; i++) {
                Tank& tank = currentPlayer.getTank(i);
                if (tank.isAlive() &&
                    tank.getPosition().row == clickRow &&
                    tank.getPosition().col == clickCol) {
                    selectedTank->setSelected(false);
                    tank.setSelected(true);
                    selectedTank = &tank;
                    clickedAnotherTank = true;
                    break;
                }
            }

            if (!clickedAnotherTank) {
                // verificar si la celda destino está ocupada
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

                if (map.getCell(clickRow, clickCol).type == CellType::free && !cellOccupied) {
                    bool usePrecision = (hasPendingPowerUp[currentTurn] &&
                        pendingPowerUp[currentTurn].type == PowerUpType::movePrecision);

                    // command pattern para mover
                    MoveCommand cmd(selectedTank, { clickRow, clickCol }, usePrecision);
                    cmd.execute(*this);
                }
            }
        }
    }

    // click derecho: disparar
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
        if (selectedTank == nullptr) return;

        int clickCol = GetMouseX() / CELL_SIZE;
        int clickRow = GetMouseY() / CELL_SIZE;

        if (clickRow < 0 || clickRow >= ROWS || clickCol < 0 || clickCol >= COLS) return;

        bool myPowerUp = hasPendingPowerUp[currentTurn];
        bool usePrecisionShot = myPowerUp && pendingPowerUp[currentTurn].type == PowerUpType::attackPrecision;
        bool fullPower = myPowerUp && pendingPowerUp[currentTurn].type == PowerUpType::attackPower;

        // command pattern para disparar
        ShootCommand cmd(selectedTank, { clickRow, clickCol }, usePrecisionShot, fullPower);
        cmd.execute(*this);
    }

    // shift: activar power-up
    if (IsKeyPressed(KEY_LEFT_SHIFT)) {
        Player& currentPlayer = (currentTurn == 1) ? player1 : player2;
        if (!currentPlayer.hasPowerUps()) return;

        // command pattern para power-up
        PowerUpCommand cmd(currentTurn);
        cmd.execute(*this);
    }
}

// blue/cyan reciben menos daño que red/yellow
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

            notify(GameEvent::Hit);
            if (!t.isAlive()) notify(GameEvent::Explosion);
        }
    }
}

void Game::update() {
    timeRemaining -= GetFrameTime();
    if (timeRemaining <= 0.0f) {
        timeRemaining = 0.0f;
        timeUp = true;
    }
    checkEliminationWin();
    checkTimeWin();
}

void Game::render() {
    renderer.beginFrame();
    renderer.drawMap(map);
    renderer.drawPath(currentPath, map);
    renderer.drawTanks(player1, player2);
    renderer.drawHub();

    if (currentBullet != nullptr) {
        renderer.drawBulletTrail(currentBullet->getBulletPath(), map);
    }

    if (currentTurn == 1) {
        DrawRectangle((GetScreenWidth() / 2 - MeasureText("Turno: Jugador 1", 20) / 2) - 5, 42.5, MeasureText("Turno: Jugador 1", 20) + 10, 25, Fade(BLUE, 0.5f));
        DrawText("Turno: Jugador 1", GetScreenWidth() / 2 - MeasureText("Turno: Jugador 1", 20) / 2, 45, 20, SKYBLUE);
    }
    else {
        DrawRectangle((GetScreenWidth() / 2 - MeasureText("Turno: Jugador 1", 20) / 2) - 5, 42.5, MeasureText("Turno: Jugador 1", 20) + 10, 25, Fade(RED, 0.5f));
        DrawText("Turno: Jugador 2", GetScreenWidth() / 2 - MeasureText("Turno: Jugador 2", 20) / 2, 45, 20, ORANGE);
    }

    renderer.drawHUD(player1, player2);
    renderer.drawPowerUps(player1, player2);
    DrawText(powerUpMessage1, 10, 800, 16, BLACK);
    DrawText(powerUpMessage2, 650, 800, 16, BLACK);

    // temporizador
    if (!gameOver) {
        int minutes = (int)(timeRemaining / 60);
        int seconds = (int)(timeRemaining) % 60;
        const char* timerText = TextFormat("%d:%02d", minutes, seconds);
        Color timerColor = (timeRemaining <= 30.0f) ? RED : WHITE;
        DrawText(timerText, GetScreenWidth() / 2 - MeasureText(timerText, 30) / 2, 8, 30, timerColor);
    }

    // pantalla de resultado
    if (gameOver) {
        DrawRectangle(250, 250, 780, 320, Fade(BLACK, 0.85f));
        DrawRectangleLines(250, 250, 780, 320, WHITE);

        const char* title = "";
        Color titleColor = WHITE;
        switch (result) {
        case GameResult::Player1Wins: title = "Gana Jugador 1!"; titleColor = BLUE;   break;
        case GameResult::Player2Wins: title = "Gana Jugador 2!"; titleColor = RED;    break;
        case GameResult::Draw:        title = "Empate!";          titleColor = YELLOW; break;
        default: break;
        }
        DrawText(title, GetScreenWidth() / 2 - MeasureText(title, 48) / 2, 290, 48, titleColor);

        const char* reason = timeUp ? "Se acabo el tiempo" : "Todos los tanques destruidos";
        DrawText(reason, GetScreenWidth() / 2 - MeasureText(reason, 22) / 2, 355, 22, LIGHTGRAY);

        DrawRectangle(565, 430, 150, 50, DARKBLUE);
        DrawRectangleLines(565, 430, 150, 50, WHITE);
        DrawText("Salir", 618, 447, 22, WHITE);
    }

    renderer.endFrame();
}

void Game::nextTurn() {
    if (extraTurns > 0) {
        extraTurns--;
        currentTurn = doubleTurnOwner;
    }
    else {
        currentTurn = (currentTurn == 1) ? 2 : 1;
        notify(GameEvent::TurnChange);

        // si hay turnos extra pendientes y ahora le toca al dueño, activarlos
        if (pendingExtraTurns > 0 && currentTurn == doubleTurnOwner) {
            extraTurns = pendingExtraTurns;
            pendingExtraTurns = 0;
        }
    }

    randomPowerUp();
}

void Game::randomPowerUp() {
    for (int p = 1; p <= 2; p++) {
        Player& player = (p == 1) ? player1 : player2;

        if (rand() % 4 != 0) continue; // 25% por jugador por turno
        if (player.getPowerUpCount() >= MAX_POWERUPS) continue;

        PowerUpType type = static_cast<PowerUpType>(rand() % 4);
        player.addPowerUp({ type });
        notify(GameEvent::PowerUpPickup);
    }
}

void Game::checkEliminationWin() {
    if (gameOver) return;

    bool p1HasTanks = player1.hasTanks();
    bool p2HasTanks = player2.hasTanks();

    if (p1HasTanks && p2HasTanks) return;

    if (!p1HasTanks && !p2HasTanks) result = GameResult::Draw;
    else if (!p1HasTanks)           result = GameResult::Player2Wins;
    else                            result = GameResult::Player1Wins;

    notify(GameEvent::BackgroundMusicStop);
    notify(GameEvent::Results);
    gameOver = true;
}

void Game::checkTimeWin() {
    if (gameOver || !timeUp) return;

    int p1Count = 0, p2Count = 0;
    for (int i = 0; i < 4; i++) {
        if (player1.getTank(i).isAlive()) p1Count++;
        if (player2.getTank(i).isAlive()) p2Count++;
    }

    if (p1Count > p2Count)      result = GameResult::Player1Wins;
    else if (p2Count > p1Count) result = GameResult::Player2Wins;
    else                        result = GameResult::Draw;

    notify(GameEvent::BackgroundMusicStop);
    notify(GameEvent::Results);
    gameOver = true;
}