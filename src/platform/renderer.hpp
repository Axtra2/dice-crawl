#pragma once

// #include <game/character/player.hpp>
// #include <item.hpp>
// #include <game/room.hpp>
// #include <game/map.hpp>
// #include <tile.hpp>

#include <unordered_map>
#include <cstdint>
#include <utility>
#include <bitset>
#include <vector>
#include <span>
#include <map>

class Player;
class Map;

class Renderer {
public:
    Renderer(void* result);

    void render(
        const Map& map,
        const Player& player,
        int32_t selectedInventorySlot
    );

    void render(
        const std::string& title,
        int32_t selectedOption,
        const std::span<std::string>& options
    );

private:
    void* data_;
};
