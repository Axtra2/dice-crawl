#include <game/character/mob/sci_fi_mob.hpp>
#include <game/character/mob/strategy/hostile.hpp>
#include <game/character/mob/strategy/passive.hpp>
#include <game/character/mob/strategy/coward.hpp>
#include <game/character/mob/strategy/mold.hpp>

Color SciFiMob::getColor() const {
    return { .r = 0x00, .g = 0xBB, .b = 0xF9 };
}

[[nodiscard]] Mob* SciFiMob::clone() const {
    SciFiMob* mob = new SciFiMob;

    // character
    mob->setXP(getXP());
    mob->setHealth(getHealth());
    mob->setMaxHealth(getMaxHealth());
    mob->getBaseAttackDiceMut() = getBaseAttackDice();
    mob->getBaseDefenseDiceMut() = getBaseDefenseDice();

    // mob
    mob->setX(getX());
    mob->setY(getY());
    mob->setStrategy(std::unique_ptr<MobStrategy>(getStrategy()->clone()));

    return mob;
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

[[nodiscard]] Mob* SciFiMobFactory::createMold() {
    Mob* mob = new SciFiMob;
    mob->setHealth(1);
    mob->setMaxHealth(1);
    mob->setXP(1);
    mob->setStrategy(std::unique_ptr<MobStrategy>(new Mold()));
    return mob;
}
