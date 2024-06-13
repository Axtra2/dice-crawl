#pragma once

#include <game/character/mob/mob.hpp>

class Coward : public MobStrategy {
public:
    Coward();
    [[nodiscard]] MobStrategy* clone() const override;

private:
    Action pickAction(const Mob& mob, const Room& room) override;
};
