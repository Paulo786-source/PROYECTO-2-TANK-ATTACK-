#include "GameCommand.h"
#include "Game.h"
#include "PathfindingStrategy.h"

// MoveCommand

void MoveCommand::execute(Game& game) {
    Position origin = tank->getPosition();

    // construir mapa de celdas bloqueadas
    bool blocked[ROWS * COLS] = {};
    game.buildBlockedArray(blocked);
    blocked[game.map.coordsToNode(origin.row, origin.col)] = false; // el tanque que se mueve no se bloquea

    int overrideProb = usePrecision ? 90 : -1;
    game.currentPath = tank->getStrategy()->calculatePath(game.map, origin, destination, blocked, overrideProb);

    if (game.currentPath.length > 0) {
        int destRow, destCol;
        game.map.nodeToCoords(game.currentPath.nodes[game.currentPath.length - 1], destRow, destCol);
        tank->setPosition({ destRow, destCol });

        if (game.currentPath.length >= 2) {
            int preRow, preCol;
            game.map.nodeToCoords(game.currentPath.nodes[game.currentPath.length - 2], preRow, preCol);
            int rowDiff = preRow - destRow;
            int colDiff = preCol - destCol;

            float angle = 0.0f;
            if (rowDiff > 0 && colDiff == 0) angle = 0.0f;
            else if (rowDiff < 0 && colDiff == 0) angle = 180.0f;
            else if (rowDiff == 0 && colDiff > 0) angle = 270.0f;
            else if (rowDiff == 0 && colDiff < 0) angle = 90.0f;
            else if (rowDiff > 0 && colDiff > 0)  angle = 315.0f;
            else if (rowDiff > 0 && colDiff < 0)  angle = 45.0f;
            else if (rowDiff < 0 && colDiff > 0)  angle = 225.0f;
            else if (rowDiff < 0 && colDiff < 0)  angle = 135.0f;
            tank->setAngle(angle);
        }
    }

    tank->setSelected(false);
    game.selectedTank = nullptr;

    if (game.currentTurn == 1) game.powerUpMessage1 = "";
    else                        game.powerUpMessage2 = "";

    // consumir el power-up solo si el movimiento tuvo �xito
    if (game.hasPendingPowerUp[game.currentTurn] && game.currentPath.length > 0) {
        game.hasPendingPowerUp[game.currentTurn] = false;
    }

    delete game.currentBullet;
    game.currentBullet = nullptr;

    game.nextTurn();
}


// ShootCommand

void ShootCommand::execute(Game& game) {
    Position origin = tank->getPosition();

    // orientar el tanque hacia el objetivo
    int rowDiff = target.row - origin.row;
    int colDiff = target.col - origin.col;
    float angle = 0.0f;
    if (rowDiff < 0 && colDiff == 0) angle = 0.0f;
    else if (rowDiff > 0 && colDiff == 0) angle = 180.0f;
    else if (rowDiff == 0 && colDiff > 0) angle = 90.0f;
    else if (rowDiff == 0 && colDiff < 0) angle = 270.0f;
    else if (rowDiff < 0 && colDiff > 0)  angle = 45.0f;
    else if (rowDiff < 0 && colDiff < 0)  angle = 315.0f;
    else if (rowDiff > 0 && colDiff > 0)  angle = 135.0f;
    else if (rowDiff > 0 && colDiff < 0)  angle = 225.0f;
    tank->setAngle(angle);

    game.currentPath = Path();

    delete game.currentBullet;
    game.currentBullet = nullptr;

    BulletPath trail;
    if (usePrecisionShot) {
        trail = Pathfinding::calculateprecisionShot(game.map, origin, target, game.player1, game.player2);
    }
    else {
        trail = Pathfinding::calculateBulletPath(game.map, origin, target, game.player1, game.player2);
    }

    game.currentBullet = new Bullet(trail, tank, fullPower);
    game.notify(GameEvent::Shot);
    game.dealBulletDamage(*game.currentBullet);

    if (game.hasPendingPowerUp[game.currentTurn]) {
        game.hasPendingPowerUp[game.currentTurn] = false;
    }

    tank->setSelected(false);
    game.selectedTank = nullptr;

    if (game.currentTurn == 1) game.powerUpMessage1 = "";
    else                        game.powerUpMessage2 = "";

    game.nextTurn();
}


// PowerUpCommand

void PowerUpCommand::execute(Game& game) {
    Player& currentPlayer = (playerTurn == 1) ? game.player1 : game.player2;

    if (game.selectedTank != nullptr) {
        game.selectedTank->setSelected(false);
        game.selectedTank = nullptr;
    }

    game.pendingPowerUp[playerTurn] = currentPlayer.usePowerUp();
    game.notify(GameEvent::PowerUpActivate);

    const char* message = "";
    switch (game.pendingPowerUp[playerTurn].type) {
    case PowerUpType::doubleTurn:      message = "Power-up: Doble Turno";             break;
    case PowerUpType::movePrecision:   message = "Power-up: Precision de Movimiento"; break;
    case PowerUpType::attackPower:     message = "Power-up: Poder de Ataque";         break;
    case PowerUpType::attackPrecision: message = "Power-up: Precision de Ataque";     break;
    default:                                                                            break;
    }

    if (playerTurn == 1) game.powerUpMessage1 = message;
    else                  game.powerUpMessage2 = message;

    if (game.pendingPowerUp[playerTurn].type == PowerUpType::doubleTurn) {
        game.pendingExtraTurns = 1;
        game.doubleTurnOwner = playerTurn;
        game.hasPendingPowerUp[playerTurn] = false;
    }
    else {
        game.hasPendingPowerUp[playerTurn] = true;
    }

    game.nextTurn();
}