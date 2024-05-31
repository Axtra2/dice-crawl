#pragma once

#include <game/dice.hpp>

#include <cstdint>

class Map;

class Character {
public:
    virtual void receiveAttack(int32_t damage) = 0;
    bool isDead() const;


    int32_t getMaxHealth() const;
    int32_t getHealth() const;
    const Dice<int32_t>& getBaseAttackDice() const;
    const Dice<int32_t>& getBaseDefenseDice() const;

protected:
    int32_t maxHealth_ = 10;
    int32_t health_ = 10;
    Dice<int32_t> baseAttackDice_ = {
        { 1, "1" },
        { 1, "1" },
        { 1, "1" },
        { 1, "1" },
        { 1, "1" },
        { 1, "1" }
    };
    Dice<int32_t> baseDefenseDice_ = {
        { 0, "0" },
        { 0, "0" },
        { 0, "0" },
        { 0, "0" },
        { 0, "0" },
        { 1, "1" }
    };
};
