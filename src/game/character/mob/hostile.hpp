#pragma once

#include <game/character/mob/mob.hpp>

class Hostile : public MobStrategy {
public:
    Hostile();
    Action pickAction(const Mob& mob, const Room& room) override;
};
