#pragma once

#include <game/character/mob/mob.hpp>

class Hostile : public MobStrategy {
public:
    Hostile();
    [[nodiscard]] MobStrategy* clone() const override;

private:
    Action pickAction(const Mob& mob, const Room& room) override;
};
