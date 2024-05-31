#pragma once

#include <game/character/mob/mob.hpp>

class Coward : public MobStrategy {
    Action pickAction(const Mob& mob, const Room& room) override;
    const std::string& getStrategyName() const override;
private:
    const std::string name_ = "Coward";
};
