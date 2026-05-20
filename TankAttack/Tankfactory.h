#pragma once
#include "Tank.h"
#include "Map.h"

// para la creación de tanques
class TankFactory {
public:

    static void createTanks(int playerNumber, Tank** out_tanks) {
        if (playerNumber == 1) {
            out_tanks[0] = new Tank({ 0,0 }, Tank::TankColor::blue);
            out_tanks[1] = new Tank({ 1,0 }, Tank::TankColor::blue);
            out_tanks[2] = new Tank({ ROWS - 1,0 }, Tank::TankColor::red);
            out_tanks[3] = new Tank({ ROWS - 2,0 }, Tank::TankColor::red);
        }
        else {
            out_tanks[0] = new Tank({ 0, COLS - 1 }, Tank::TankColor::cyan);
            out_tanks[1] = new Tank({ 1, COLS - 1 }, Tank::TankColor::cyan);
            out_tanks[2] = new Tank({ ROWS - 1, COLS - 1 }, Tank::TankColor::yellow);
            out_tanks[3] = new Tank({ ROWS - 2, COLS - 1 }, Tank::TankColor::yellow);
        }
    }
};