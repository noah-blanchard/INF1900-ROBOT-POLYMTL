#pragma once
#include <stdint.h>

enum class Orientation
{
    NORTH,
    EAST,
    SOUTH,
    WEST,
    FINISHED
};

struct Move
{
    Orientation orientation;
    uint8_t x;
    uint8_t y;
};
