#include <game/dice.hpp>
#include <game/item.hpp>

#include <cassert>

void testItemInfo() {
    ItemInfo info;

    assert(!info.isWearable());
    using enum ItemInfo::WearInfo::WearType;
    info.setWearType(HAND);
    assert(info.isWearable());
    assert(info.getWearType() == HAND);

    info.setName("a");
    assert(info.getName() == "a");

    info.setW(0.1);
    assert(info.getW() == 0.1);

    {
        int c = 0;
        assert(!info.hasAttackDice());
        info.addAttackDiceSide([&](auto){return ++c;}, "a");
        assert(info.hasAttackDice());
        const auto& dice = info.getAttackDice();
        assert(dice.size() == 1);
        assert(dice[0].second == "a");
        assert(c == 0);
        throwDice(dice)(0);
        assert(c == 1);
    }

    {
        int c = 0;
        assert(!info.hasDefenseDice());
        info.addDefenseDiceSide([&](auto){return ++c;}, "a");
        assert(info.hasDefenseDice());
        const auto& dice = info.getDefenseDice();
        assert(dice.size() == 1);
        assert(dice[0].second == "a");
        assert(c == 0);
        throwDice(dice)(0);
        assert(c == 1);
    }
}

int main() {
    testItemInfo();
}
