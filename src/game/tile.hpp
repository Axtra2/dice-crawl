#pragma once

#include <unordered_map>
#include <cstdint>

using TileID = int32_t;

struct TileInfo {
    static constexpr TileID EMPTY = 0;
    double w = 0.0;
};

const std::unordered_map<TileID, TileInfo>& getTilesDict();
