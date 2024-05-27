#include <player.hpp>
#include <random.hpp>
#include <dice.hpp>

#include <array>

int32_t Player::throwDiceForAttack() const {
    int32_t attack = throwDice(baseAttackDice);
    std::array<std::optional<ItemID>, 5> places = {
        hand, head, torso, legs, feet
    };
    for (const auto& place : places) {
        if (place) {
            auto& d = getItemsDict()
                .at(place.value())
                .wearInfo.value()
                .attackDice;
            if (d) {
                attack = throwDice(d.value())(attack);
            }
        }
    }
    return attack;
}

int32_t Player::throwDiceForDefense() const {
    int32_t defense = throwDice(baseDefenseDice);
    std::array<std::optional<ItemID>, 5> places = {
        hand, head, torso, legs, feet
    };
    for (const auto& place : places) {
        if (place) {
            auto& d = getItemsDict()
                .at(place.value())
                .wearInfo.value()
                .defenseDice;
            if (d) {
                defense = throwDice(d.value())(defense);
            }
        }
    }
    return defense;
}
