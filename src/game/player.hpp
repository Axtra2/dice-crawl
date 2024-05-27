#pragma once

#include <dice.hpp>
#include <item.hpp>
#include <tile.hpp>

#include <cstddef>
#include <cstdint>
#include <map>

struct Player {
    int32_t x = 0;
    int32_t y = 0;

    bool isDead = false;

    int32_t health = 10;
    int32_t maxHealth = 10;

    Dice<int32_t> baseAttackDice = {
        0, 1, 2, 3, 4, 5
    };

    Dice<int32_t> baseDefenseDice = {
        0, 1, 2, 3, 4, 5
    };

    int32_t maxInventorySize = 10;
    int32_t selectedInventorySlot = 0;
    std::vector<ItemID> inventory = {};

    std::optional<ItemID> hand = std::nullopt;
    std::optional<ItemID> head = std::nullopt;
    std::optional<ItemID> torso = std::nullopt;
    std::optional<ItemID> legs = std::nullopt;
    std::optional<ItemID> feet = std::nullopt;

    int32_t throwDiceForAttack() const;
    int32_t throwDiceForDefense() const;
};