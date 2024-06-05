#include <game/character/mob/fantasy_mob.hpp>
#include <game/character/mob/strategy/hostile.hpp>
#include <game/character/mob/strategy/passive.hpp>
#include <game/character/mob/strategy/coward.hpp>

Color FantasyMob::getColor() const {
    return { .r = 0xE6, .g = 0x39, .b = 0x46 };
}

[[nodiscard]] Mob* FantasyMobFactory::createHostile() {
    Mob* mob = new FantasyMob;
    mob->setStrategy(std::unique_ptr<MobStrategy>(new Hostile()));
    return mob;
}

[[nodiscard]] Mob* FantasyMobFactory::createPassive() {
    Mob* mob = new FantasyMob;
    mob->setStrategy(std::unique_ptr<MobStrategy>(new Passive()));
    return mob;
}

[[nodiscard]] Mob* FantasyMobFactory::createCoward() {
    Mob* mob = new FantasyMob;
    mob->setStrategy(std::unique_ptr<MobStrategy>(new Coward()));
    return mob;
}
