#pragma once

#include <item.hpp>
#include <tile.hpp>

#include <cstdint>
#include <map>

struct Player {
    int32_t x = 0;
    int32_t y = 0;

    int32_t attack = 0;
    int32_t health = 10;
    int32_t maxHealth = 10;

    int32_t maxInventorySize = 0;
    std::vector<ItemID> inventory = {};

    std::optional<ItemID> hand = std::nullopt;
    std::optional<ItemID> head = std::nullopt;
    std::optional<ItemID> torso = std::nullopt;
    std::optional<ItemID> legs = std::nullopt;
    std::optional<ItemID> feet = std::nullopt;

    int32_t calcAttack() const;
    int32_t calcMaxHealth() const;
};