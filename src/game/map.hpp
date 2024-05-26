#pragma once

#include <room.hpp>

#include <optional>
#include <vector>

struct Map {
    std::vector<Room> rooms;
};

std::optional<Map> loadMap(const char* filename);
