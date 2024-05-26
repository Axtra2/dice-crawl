#include <algorithm>
#include <cassert>

#include "player.hpp"
#include "item.hpp"

int main() {
    constexpr ItemID SWORD_ID = 1;
    Player p;
    assert(p.maxHealth == p.calcMaxHealth());

    assert(p.calcAttack() == p.attack);
    p.hand = SWORD_ID;

    assert(p.calcAttack() == getItemsDict().at(SWORD_ID).wearInfo->attackModifier.value()(p.attack));
    
}