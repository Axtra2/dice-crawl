#pragma once

#include <game/character/mob/mob.hpp>

class Passive : public MobStrategy {
public:
    Passive();
    Action pickAction(const Mob& mob, const Room& room) override;
};
