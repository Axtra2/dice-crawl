#include <renderer.hpp>
#include <roaming.hpp>
#include <state.hpp>

#include <cstdint>
#include <random>

bool Roaming::init() {
    static auto rng = std::mt19937(std::random_device{}());
    room_ = generateRoom(21, 21, rng);
    player_.x = room_.width / 2;
    player_.y = room_.height - 1;
    return true;
}

State* Roaming::update(char c) {
    switch (c) {
    case 'w':
        playerMoveUp();
        break;
    case 'a':
        playerMoveLeft();
        break;
    case 's':
        playerMoveDown();
        break;
    case 'd':
        playerMoveRight();
        break;
    case 'p':
        playerPickup();
        break;
    case 'e':
        playerEquip();
        break;
    case 'k':
        inventorySelectionUp();
        break;
    case 'j':
        inventorySelectionDown();
        break;
    }
    return this;
}

bool Roaming::render(Renderer& renderer) {
    return renderer.render(room_, player_);
}

void Roaming::playerMoveUp() {
    int32_t oldY = player_.y;
    player_.y = std::clamp(player_.y - 1, 0, room_.height - 1);
    auto it = room_.tiles.find({player_.x, player_.y});
    if (it != room_.tiles.end() && it->second == '#') {
        player_.y = oldY;
    }
}

void Roaming::playerMoveDown() {
    int32_t oldY = player_.y;
    player_.y = std::clamp(player_.y + 1, 0, room_.height - 1);
    auto it = room_.tiles.find({player_.x, player_.y});
    if (it != room_.tiles.end() && it->second == '#') {
        player_.y = oldY;
    }
}

void Roaming::playerMoveLeft() {
    int32_t oldX = player_.x;
    player_.x = std::clamp(player_.x - 1, 0, room_.width - 1);
    auto it = room_.tiles.find({player_.x, player_.y});
    if (it != room_.tiles.end() && it->second == '#') {
        player_.x = oldX;
    }
}

void Roaming::playerMoveRight() {
    int32_t oldX = player_.x;
    player_.x = std::clamp(player_.x + 1, 0, room_.width - 1);
    auto it = room_.tiles.find({player_.x, player_.y});
    if (it != room_.tiles.end() && it->second == '#') {
        player_.x = oldX;
    }
}

void Roaming::playerEquip() {
    if (player_.selectedInventorySlot < 5) {
        playerUnequip();
        return;
    }
    if (player_.inventory.empty()) {
        return;
    }
    int32_t slot = player_.selectedInventorySlot - 5;
    ItemID itemID = player_.inventory[slot];
    const ItemInfo& itemInfo = getItemsDict().at(itemID);
    if (!itemInfo.wearInfo) {
        return;
    }
    using enum ItemInfo::WearInfo::WearType;
    switch (itemInfo.wearInfo.value().wearType) {
    case HAND:
        if (!player_.hand) {
            player_.inventory.erase(player_.inventory.begin() + slot);
            player_.hand = itemID;
        }
        break;
    case HEAD:
        // TODO
        break;
    case TORSO:
        // TODO
        break;
    case LEGS:
        // TODO
        break;
    case FEET:
        // TODO
        break;
    }
    player_.selectedInventorySlot = std::clamp(
        player_.selectedInventorySlot,
        0,
        static_cast<int32_t>(player_.inventory.size() + 4)
    );
}

void Roaming::playerUnequip() {
    if (player_.inventory.size() >= player_.maxInventorySize) {
        return;
    }
    auto moveToInventory = [&](auto& v){
        if (v) {
            player_.inventory.push_back(v.value());
            v.reset();
        }
    };
    switch (player_.selectedInventorySlot) {
    case 0: moveToInventory(player_.hand); break;
    case 1: moveToInventory(player_.head); break;
    case 2: moveToInventory(player_.torso); break;
    case 3: moveToInventory(player_.legs); break;
    case 4: moveToInventory(player_.feet); break;
    }
}

void Roaming::playerPickup() {
    if (player_.inventory.size() >= player_.maxInventorySize) {
        return;
    }
    auto it = room_.items.find({player_.x, player_.y});
    if (it != room_.items.end()) {
        player_.inventory.push_back(it->second);
        room_.items.erase(it);
    }
}

void Roaming::inventorySelectionUp() {
    player_.selectedInventorySlot = std::clamp(
        player_.selectedInventorySlot - 1,
        0,
        static_cast<int32_t>(player_.inventory.size() + 4)
    );
}

void Roaming::inventorySelectionDown() {
    player_.selectedInventorySlot = std::clamp(
        player_.selectedInventorySlot + 1,
        0,
        static_cast<int32_t>(player_.inventory.size() + 4)
    );
}
