#pragma once

#include <player.hpp>
#include <item.hpp>
#include <room.hpp>
#include <tile.hpp>

#include <unordered_map>
#include <cstdint>
#include <utility>
#include <bitset>
#include <vector>
#include <map>

class Renderer {
public:
    Renderer(void* result);

    bool render(
        const Room& room,
        const Player& player,
        const std::unordered_map<int32_t, TileInfo>& tilesDict = getTilesDict(),
        const std::unordered_map<int32_t, ItemInfo>& itemsDict = getItemsDict()
    );

private:
    void* data_;
};
