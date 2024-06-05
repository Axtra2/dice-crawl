#include <game/character/mob/fantasy_mob.hpp>
#include <game/character/mob/mob.hpp>
#include <game/room.hpp>

#include <cassert>
#include <memory>

void testReceiveAttack() {
    FantasyMob mob;
    assert(mob.getHealth() == mob.getMaxHealth());
    assert(!mob.isDead());

    mob.receiveAttack(0);
    assert(mob.getHealth() == mob.getMaxHealth());

    auto& defenseDice = mob.getBaseDefenseDice();
    auto it = std::max_element(defenseDice.begin(), defenseDice.end());
    assert(it != defenseDice.end());

    int32_t attack = mob.getMaxHealth() + it->first;
    mob.receiveAttack(attack);
    assert(mob.getHealth() == 0);
    assert(mob.isDead());
}

void testMove() {
    FantasyMob mob;
    mob.setX(0);
    mob.setY(0);
    assert(mob.getX() == 0);
    assert(mob.getY() == 0);
}

void testStrategy() {
    struct S : MobStrategy {
        int c = 0;
        S() : MobStrategy("a") { }
        Action pickAction(const Mob&, const Room&) {
            ++c;
            return {};
        }
    };
    FantasyMob mob;
    Room room;
    S* strat = new S;
    mob.setStrategy(std::unique_ptr<MobStrategy>(strat));
    assert(mob.getStrategyName() == strat->getName());
    assert(strat->c == 0);
    mob.pickAction(room);
    assert(strat->c == 1);
}

int main() {
    testReceiveAttack();
    testMove();
    testStrategy();
}
