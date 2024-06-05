#pragma once

#include <game/character/mob/mob.hpp>

class FantasyMob : public Mob {
public:
    Color getColor() const override;
};

class FantasyMobFactory : public MobFactory {
public:
    [[nodiscard]] Mob* createHostile() override;
    [[nodiscard]] Mob* createPassive() override;
    [[nodiscard]] Mob* createCoward() override;
};
