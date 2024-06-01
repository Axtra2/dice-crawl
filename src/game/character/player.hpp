#pragma once

#include <game/character/character.hpp>
#include <game/direction.hpp>
#include <game/item.hpp>

#include <optional>
#include <cstddef>
#include <cstdint>
#include <array>

class Room;

class Player : public Character {
public:
    using EquipmentSlot = ItemInfo::WearInfo::WearType;

    static constexpr int32_t N_INV_SLOTS = 10;
    static constexpr int32_t N_EQ_SLOTS = static_cast<int32_t>(EquipmentSlot::N_WEAR_TYPES);

    void receiveAttack(int32_t damage) override;

    bool move(Map& map, Direction direction);
    bool pickUp(Room& room);
    bool equip(int32_t inventorySlot);
    bool unequip(EquipmentSlot eqSlot);
    bool canLevelUp() const;
    int32_t getLevel() const;
    void levelUp();

    int32_t getMaxHealth() const;
    void setMaxHealth(int32_t maxHealth);

    int32_t getHealth() const;
    void setHealth(int32_t health);

    const std::array<std::optional<int32_t>, N_EQ_SLOTS>& getEquipment() const;
    const std::array<std::optional<int32_t>, N_INV_SLOTS>& getInventory() const;

private:
    std::array<std::optional<int32_t>, N_EQ_SLOTS> equipment_;
    std::array<std::optional<int32_t>, N_INV_SLOTS> inventory_;
    int32_t nItemsInInventory_ = 0;
    int32_t level_ = 0;
};
