#include <game/character/mob/fantasy_mob.hpp>
#include <game/character/mob/strategy/hostile.hpp>
#include <game/character/mob/strategy/passive.hpp>
#include <game/character/mob/strategy/coward.hpp>
#include <game/character/mob/strategy/mold.hpp>

Color FantasyMob::getColor() const {
    return { .r = 0xE6, .g = 0x39, .b = 0x46 };
}

[[nodiscard]] Mob* FantasyMob::clone() const {
    FantasyMob* mob = new FantasyMob;

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

[[nodiscard]] Mob* FantasyMobFactory::createMold() {
    Mob* mob = new FantasyMob;
    mob->setStrategy(std::unique_ptr<MobStrategy>(new Mold()));
    mob->setHealth(1);
    mob->setMaxHealth(1);
    mob->setXP(1);
    return mob;
}
