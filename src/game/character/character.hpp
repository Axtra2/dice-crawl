#pragma once

#include <game/dice.hpp>

#include <cstdint>

class Map;

class Character {
public:
    virtual void receiveAttack(int32_t damage) = 0;
    virtual ~Character() = default;
    bool isDead() const;

    int32_t getMaxHealth() const;
    int32_t getHealth() const;
    void setMaxHealth(int32_t maxHealth);
    void setHealth(int32_t health);

    const Dice<int32_t>& getBaseAttackDice() const;
    const Dice<int32_t>& getBaseDefenseDice() const;
    Dice<int32_t>& getBaseAttackDiceMut();
    Dice<int32_t>& getBaseDefenseDiceMut();
    uint64_t getXP() const;
    void setXP(uint64_t xp);

private:
    int32_t maxHealth_ = 10;
    int32_t health_ = 10;
    uint64_t xp_ = 0;
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
