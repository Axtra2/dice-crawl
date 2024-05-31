#include <game/character/character.hpp>

bool Character::isDead() const {
    return health_ == 0;
}

int32_t Character::getMaxHealth() const {
    return maxHealth_;
}

int32_t Character::getHealth() const {
    return health_;
}

const Dice<int32_t>& Character::getBaseAttackDice() const {
    return baseAttackDice_;
}

const Dice<int32_t>& Character::getBaseDefenseDice() const {
    return baseDefenseDice_;
}
