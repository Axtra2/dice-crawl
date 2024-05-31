#include <game/character/player.hpp>
#include <utils/inrange.hpp>
#include <game/item.hpp>
#include <game/room.hpp>
#include <game/dice.hpp>
#include <game/map.hpp>

#include <algorithm>
#include <cstddef>
#include <utility>

void Player::receiveAttack(int32_t damage) {
    int32_t defense = throwDice(baseDefenseDice_);
    for (auto eq : equipment_) {
        if (!eq) {
            continue;
        }
        auto& item = getItemsDict().at(eq.value());
        if (!item.hasDefenseDice()) {
            continue;
        }
        const auto& dice = item.getDefenseDice();
        defense = throwDice(dice)(defense);
    }
    damage = std::max(0, damage - defense);
    addInRange(health_, -damage, 0, maxHealth_);
}

void Player::move(Map& map, Direction direction) {
    auto [dx, dy] = dirToDXDY[static_cast<size_t>(direction)];
    Room& room = map.currentRoom();
    int32_t x = room.getPlayerX() + dx;
    int32_t y = room.getPlayerY() + dy;
    if (room.isDoor(x, y) && map.currentRoomHasNeighbour(direction)) {
        map.goToNeighbour(direction);
        return;
    }
    if (!room.isPassable(x, y)) {
        return;
    }
    auto mobID = room.mobAt(x, y);
    if (mobID) {
        int32_t attack = throwDice(baseAttackDice_);
        for (auto eq : equipment_) {
            if (!eq) {
                continue;
            }
            auto& item = getItemsDict().at(eq.value());
            if (!item.hasAttackDice()) {
                continue;
            }
            const auto& dice = item.getAttackDice();
            attack = throwDice(dice)(attack);
        }
        room.getMobs()[mobID.value()].receiveAttack(attack);
        return;
    }
    room.setPlayerX(x);
    room.setPlayerY(y);
}

void Player::pickUp(Room& room) {
    if (nItemsInInventory_ >= N_INV_SLOTS) {
        return;
    }
    int32_t x = room.getPlayerX();
    int32_t y = room.getPlayerY();
    auto item = room.removeItem(x, y);
    if (!item) {
        return;
    }
    for (auto& slot : inventory_) {
        if (!slot) {
            slot = item.value();
            ++nItemsInInventory_;
            return;
        }
    }
}

void Player::equip(int32_t inventorySlot) {
    assert(inventorySlot < inventory_.size());
    auto& invSlot = inventory_[inventorySlot];
    if (!invSlot) {
        return;
    }
    int32_t itemID = invSlot.value();
    const ItemInfo& itemInfo = getItemsDict().at(itemID);
    if (!itemInfo.isWearable()) {
        return;
    }
    int32_t eqSlotI = static_cast<int32_t>(itemInfo.getWearType());
    auto& eqSlot = equipment_[eqSlotI];
    if (eqSlot) {
        return;
    }
    eqSlot = itemID;
    --nItemsInInventory_;
    inventory_[inventorySlot].reset();
}

void Player::unequip(EquipmentSlot eqSlot) {
    if (nItemsInInventory_ >= N_INV_SLOTS) {
        return;
    }
    int32_t eqSlotI = static_cast<int32_t>(eqSlot);
    auto& slot = equipment_[eqSlotI];
    if (!slot) {
        return;
    }
    for (auto& invSlot : inventory_) {
        if (!invSlot) {
            invSlot = slot.value();
            ++nItemsInInventory_;
            slot.reset();
            return;
        }
    }
}

const std::array<std::optional<int32_t>, Player::N_EQ_SLOTS>&
Player::getEquipment() const {
    return equipment_; 
}

const std::array<std::optional<int32_t>, Player::N_INV_SLOTS>&
Player::getInventory() const {
    return inventory_;
}
