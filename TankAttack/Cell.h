#pragma once

enum class CellType {
    free,
    obstacle
};

struct Cell {
    CellType type;
    bool hasTank;
    bool hasBullet;

    Cell() : type(CellType::free), hasTank(false), hasBullet(false) {}
};