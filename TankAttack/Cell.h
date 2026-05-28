#pragma once

enum class CellType {
    free,
    obstacle
};

struct Cell {
    CellType type;

    Cell() : type(CellType::free) {}
};