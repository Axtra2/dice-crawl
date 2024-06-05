#include <game/character/player.hpp>
#include <utils/inrange.hpp>
#include <game/item.hpp>
#include <game/room.hpp>
#include <game/dice.hpp>
#include <game/map.hpp>

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <utility>

void Player::receiveAttack(int32_t damage) {
    int32_t defense = throwDice(getBaseDefenseDice());
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
    int32_t h = getHealth();
    addInRange(h, -damage, 0, getMaxHealth());
    setHealth(h);
}

bool Player::move(Map& map, Direction direction) {
    auto [dx, dy] = dirToDXDY[static_cast<size_t>(direction)];
    Room& room = map.currentRoom();
    int32_t x = room.getPlayerX() + dx;
    int32_t y = room.getPlayerY() + dy;
    if (room.isDoor(x, y) && map.currentRoomHasNeighbour(direction)) {
        map.goToNeighbour(direction);
        return true;
    }
    if (!room.isPassable(x, y)) {
        return false;
    }
    Character* otherCharacter = room.characterAt(x, y);
    if (otherCharacter != nullptr) {
        int32_t attack = throwDice(getBaseAttackDice());
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
        otherCharacter->receiveAttack(attack);
        if (otherCharacter->isDead()) {
            setXP(getXP() + otherCharacter->getXP());
        }
        return true;
    }
    room.setPlayerX(x);
    room.setPlayerY(y);
    return true;
}

bool Player::pickUp(Room& room) {
    if (nItemsInInventory_ >= N_INV_SLOTS) {
        return false;
    }
    int32_t x = room.getPlayerX();
    int32_t y = room.getPlayerY();
    auto item = room.removeItem(x, y);
    if (!item) {
        return false;
    }
    for (auto& slot : inventory_) {
        if (!slot) {
            slot = item.value();
            ++nItemsInInventory_;
            return true;
        }
    }
    return false;
}

bool Player::equip(int32_t inventorySlot) {
    assert(inventorySlot < inventory_.size());
    auto& invSlot = inventory_[inventorySlot];
    if (!invSlot) {
        return false;
    }
    int32_t itemID = invSlot.value();
    const ItemInfo& itemInfo = getItemsDict().at(itemID);
    if (!itemInfo.isWearable()) {
        return false;
    }
    int32_t eqSlotI = static_cast<int32_t>(itemInfo.getWearType());
    auto& eqSlot = equipment_[eqSlotI];
    if (eqSlot) {
        return false;
    }
    eqSlot = itemID;
    --nItemsInInventory_;
    inventory_[inventorySlot].reset();
    return true;
}

bool Player::unequip(EquipmentSlot eqSlot) {
    if (nItemsInInventory_ >= N_INV_SLOTS) {
        return false;
    }
    int32_t eqSlotI = static_cast<int32_t>(eqSlot);
    auto& slot = equipment_[eqSlotI];
    if (!slot) {
        return false;
    }
    for (auto& invSlot : inventory_) {
        if (!invSlot) {
            invSlot = slot.value();
            ++nItemsInInventory_;
            slot.reset();
            return true;
        }
    }
    return false;
}

const std::array<std::optional<int32_t>, Player::N_EQ_SLOTS>&
Player::getEquipment() const {
    return equipment_; 
}

const std::array<std::optional<int32_t>, Player::N_INV_SLOTS>&
Player::getInventory() const {
    return inventory_;
}

bool Player::canLevelUp() const {
    return getXP() >= xpForLevelUp(level_);
}

int32_t Player::getLevel() const {
    return level_;
}

uint64_t Player::xpForLevelUp(int32_t currentLevel) {
    return 1ull << currentLevel;
}

void Player::oneLevelUp() {
    assert(canLevelUp());
    setXP(getXP() - xpForLevelUp(level_));
    ++level_;
}
