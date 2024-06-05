#pragma once

#include <game/character/mob/mob.hpp>

class Passive : public MobStrategy {
public:
    Passive();
    [[nodiscard]] MobStrategy* clone() const override;

private:
    Action pickAction(const Mob& mob, const Room& room) override;
};
