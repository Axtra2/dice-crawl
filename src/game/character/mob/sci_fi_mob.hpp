#pragma once

#include <game/character/mob/mob.hpp>

class SciFiMob : public Mob {
public:
    Color getColor() const override;
};

class SciFiMobFactory : public MobFactory {
public:
    [[nodiscard]] Mob* createHostile() override;
    [[nodiscard]] Mob* createPassive() override;
    [[nodiscard]] Mob* createCoward() override;
};
