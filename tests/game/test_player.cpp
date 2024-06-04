#include <game/character/player.hpp>
#include <game/room.hpp>
#include <game/item.hpp>

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <sstream>
#include <string>

void testReceiveAttack() {
    Player p;
    assert(p.getHealth() == p.getMaxHealth());
    assert(!p.isDead());

    p.receiveAttack(0);
    assert(p.getHealth() == p.getMaxHealth());

    auto& defenseDice = p.getBaseDefenseDice();
    auto it = std::max_element(defenseDice.begin(), defenseDice.end());
    assert(it != defenseDice.end());

    int32_t attack = p.getMaxHealth() + it->first;
    p.receiveAttack(attack);
    assert(p.getHealth() == 0);
    assert(p.isDead());
}

void testHealth() {
    Player p;

    p.setMaxHealth(2);
    assert(p.getMaxHealth() == 2);

    p.setHealth(1);
    assert(p.getHealth() == 1);

    p.setHealth(2);
    assert(p.getHealth() == 2);

    p.setMaxHealth(1);
    assert(p.getMaxHealth() == 1);
    assert(p.getHealth() == 1);

    p.setHealth(2);
    assert(p.getMaxHealth() == 1);
    assert(p.getHealth() == 1);
}

void testPickUp() {
    std::stringstream ss(R"(
        1 1
        0
        1
        0 0 1
    )");
    Room room;
    room.load(ss);
    auto itemID = room.getItems().at({0,0});

    Player p;
    p.pickUp(room);
    assert(p.getInventory().at(0) == itemID);

    auto it = room.getItems().find({0,0});
    assert(it == room.getItems().end());
}

void testEquip() {
    for (const auto& [itemID, itemInfo] : getItemsDict()) {
        if (!itemInfo.isWearable()) {
            continue;
        }
        using namespace std::string_literals;
        std::stringstream ss(R"(
            1 1
            0
            1
            0 0 )"s + std::to_string(itemID)
        );
        Room room;
        room.load(ss);
        Player p;
        p.pickUp(room);

        size_t eqI = static_cast<size_t>(itemInfo.getWearType());
        const auto& eqSlot = p.getEquipment().at(eqI);
        const auto& invSlot = p.getInventory().at(0);

        p.equip(0);
        assert(eqSlot.has_value());
        assert(eqSlot.value() == itemID);
        assert(!invSlot.has_value());

        p.unequip(itemInfo.getWearType());
        assert(!eqSlot.has_value());
        assert(invSlot.has_value());
        assert(invSlot.value() == itemID);

        break;
    }
}

int main() {
    testReceiveAttack();
    testHealth();
    testPickUp();
    testEquip();
}
