#pragma once

#include <game/character/mob/mob.hpp>

class Coward : public MobStrategy {
public:
    Coward();
    Action pickAction(const Mob& mob, const Room& room) override;
};
