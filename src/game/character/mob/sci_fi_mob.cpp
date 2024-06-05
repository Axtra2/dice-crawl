#include <game/character/mob/sci_fi_mob.hpp>
#include <game/character/mob/strategy/hostile.hpp>
#include <game/character/mob/strategy/passive.hpp>
#include <game/character/mob/strategy/coward.hpp>

Color SciFiMob::getColor() const {
    return { .r = 0x00, .g = 0xBB, .b = 0xF9 };
}

[[nodiscard]] Mob* SciFiMobFactory::createHostile() {
    Mob* mob = new SciFiMob;
    mob->setStrategy(std::unique_ptr<MobStrategy>(new Hostile()));
    return mob;
}

[[nodiscard]] Mob* SciFiMobFactory::createPassive() {
    Mob* mob = new SciFiMob;
    mob->setStrategy(std::unique_ptr<MobStrategy>(new Passive()));
    return mob;
}

[[nodiscard]] Mob* SciFiMobFactory::createCoward() {
    Mob* mob = new SciFiMob;
    mob->setStrategy(std::unique_ptr<MobStrategy>(new Coward()));
    return mob;
}
