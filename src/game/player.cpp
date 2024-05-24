#include <player.hpp>

int32_t Player::calcAttack() const {
    auto res = attack;
    if (hand) {
        auto m = getItemsDict()
            .at(hand.value())
            .wearInfo.value()
            .attackModifier;
        if (m) {
            res = m.value()(res);
        }
    }
    return res;
}

int32_t Player::calcMaxHealth() const {
    auto res = maxHealth;
    if (hand) {
        auto m = getItemsDict()
            .at(hand.value())
            .wearInfo.value()
            .maxHealthModifier;
        if (m) {
            res = m.value()(res);
        }
    }
    return res;
}
