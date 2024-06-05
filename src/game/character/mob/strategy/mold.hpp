#pragma once

#include <game/character/mob/mob.hpp>

class Mold : public MobStrategy {
public:
    Mold();
    [[nodiscard]] MobStrategy* clone() const override;

private:
    Action pickAction(const Mob& mob, const Room& room) override;
};
