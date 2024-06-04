#include <game/character/character.hpp>

#include <algorithm>

bool Character::isDead() const {
    return health_ == 0;
}

int32_t Character::getMaxHealth() const {
    return maxHealth_;
}

int32_t Character::getHealth() const {
    return health_;
}

void Character::setMaxHealth(int32_t maxHealth) {
    maxHealth_ = std::max(0, maxHealth);
    health_ = std::clamp(health_, 0, maxHealth);
}

void Character::setHealth(int32_t health) {
    health_ = std::clamp(health, 0, maxHealth_);
}

const Dice<int32_t>& Character::getBaseAttackDice() const {
    return baseAttackDice_;
}

const Dice<int32_t>& Character::getBaseDefenseDice() const {
    return baseDefenseDice_;
}

Dice<int32_t>& Character::getBaseAttackDiceMut() {
    return baseAttackDice_;
}

Dice<int32_t>& Character::getBaseDefenseDiceMut() {
    return baseDefenseDice_;
}

uint64_t Character::getXP() const {
    return xp_;
}

void Character::setXP(uint64_t xp) {
    xp_ = std::max(0ull, xp);
}
