#include <player.hpp>

#include <array>

int32_t Player::calcAttack() const {
    auto res = attack;
    std::array<std::optional<ItemID>, 5> places = {
        hand, head, torso, legs, feet
    };
    for (const auto& place : places) {
        if (place) {
            auto& m = getItemsDict()
                .at(place.value())
                .wearInfo.value()
                .attackModifier;
            if (m) {
                res = m.value()(res);
            }
        }
    }
    return res;
}

int32_t Player::calcMaxHealth() const {
    auto res = maxHealth;
    std::array<std::optional<ItemID>, 5> places = {
        hand, head, torso, legs, feet
    };
    for (const auto& place : places) {
        if (place) {
            auto& m = getItemsDict()
                .at(place.value())
                .wearInfo.value()
                .maxHealthModifier;
            if (m) {
                res = m.value()(res);
            }
        }
    }
    return res;
}
