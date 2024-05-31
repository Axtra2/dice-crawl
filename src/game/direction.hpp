#pragma once

#include <cstddef>
#include <cstdint>
#include <utility>

enum class Direction {
    EAST = 0, NORTH, WEST, SOUTH, NDIRS
};

static constexpr std::pair<int32_t, int32_t>
dirToDXDY[static_cast<size_t>(Direction::NDIRS)] = {
    std::pair{ 1, 0},
    std::pair{ 0,-1},
    std::pair{-1, 0},
    std::pair{ 0, 1}
};

static constexpr Direction
oppositeDir[static_cast<size_t>(Direction::NDIRS)] = {
    Direction::WEST,
    Direction::SOUTH,
    Direction::EAST,
    Direction::NORTH
};

Direction dirFromDXDY(int32_t dx, int32_t dy);
