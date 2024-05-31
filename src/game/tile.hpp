#pragma once

#include <unordered_map>
#include <cstdint>

using TileID = int32_t;

struct TileInfo {
    double w = 0.0;
    bool isWall = false;
};

const std::unordered_map<TileID, TileInfo>& getTilesDict();
