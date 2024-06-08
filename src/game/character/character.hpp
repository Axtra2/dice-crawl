#pragma once

#include <game/dice.hpp>

#include <cstdint>

class Map;

class Character {
public:
    virtual void receiveAttack(int32_t damage) = 0;
    virtual ~Character() = default;

    virtual bool isDead() const;

    virtual int32_t getMaxHealth() const;
    virtual int32_t getHealth() const;
    virtual void setMaxHealth(int32_t maxHealth);
    virtual void setHealth(int32_t health);

    virtual const Dice<int32_t>& getBaseAttackDice() const;
    virtual const Dice<int32_t>& getBaseDefenseDice() const;
    virtual Dice<int32_t>& getBaseAttackDiceMut();
    virtual Dice<int32_t>& getBaseDefenseDiceMut();
    virtual uint64_t getXP() const;
    virtual void setXP(uint64_t xp);

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
