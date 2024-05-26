#include <esc_menu.hpp>
#include <renderer.hpp>
#include <roaming.hpp>
#include <state.hpp>

#include <cassert>
#include <cstdint>
#include <utility>
#include <random>

Roaming::Roaming(Map map)
  : map_(std::move(map))
{ }

bool Roaming::init() {
    player_ = Player();
    player_.x = map_.rooms.at(curRoom_).width / 2;
    player_.y = map_.rooms.at(curRoom_).height - 1;
    if (!escMenu_) {
        escMenu_ = new EscMenu(this);
    }
    curRoom_ = 0;
    return escMenu_->init();
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
    case 'q':
        return escMenu_;
    }
    return this;
}

bool Roaming::render(Renderer& renderer) {
    return renderer.render(map_.rooms.at(curRoom_), player_);
}

void Roaming::goToRoom(int32_t roomID, Direction fromDir) {
    const Room& nextRoom = map_.rooms.at(roomID);
    using enum Roaming::Direction;
    switch (fromDir) {
    case EAST:
        player_.x = nextRoom.width - 1;
        player_.y = nextRoom.height / 2;
        break;
    case NORTH:
        player_.x = nextRoom.width / 2;
        player_.y = 0;
        break;
    case WEST:
        player_.x = 0;
        player_.y = nextRoom.height / 2;
        break;
    case SOUTH:
        player_.x = nextRoom.width / 2;
        player_.y = nextRoom.height - 1;
        break;
    }
    curRoom_ = roomID;
}

void Roaming::playerMoveUp() {
    const Room& room = map_.rooms.at(curRoom_);
    if (room.n && player_.x == room.width / 2 && player_.y - 1 == -1) {
        using enum Roaming::Direction;
        goToRoom(room.n.value(), SOUTH);
        return;
    }
    int32_t oldY = player_.y;
    player_.y = std::clamp(player_.y - 1, 0, room.height - 1);
    auto it = room.tiles.find({player_.x, player_.y});
    if (it != room.tiles.end() && it->second == '#') {
        player_.y = oldY;
    }
}

void Roaming::playerMoveDown() {
    const Room& room = map_.rooms.at(curRoom_);
    if (room.s && player_.x == room.width / 2 && player_.y + 1 == room.height) {
        using enum Roaming::Direction;
        goToRoom(room.s.value(), NORTH);
        return;
    }
    int32_t oldY = player_.y;
    player_.y = std::clamp(player_.y + 1, 0, map_.rooms.at(curRoom_).height - 1);
    auto it = map_.rooms.at(curRoom_).tiles.find({player_.x, player_.y});
    if (it != map_.rooms.at(curRoom_).tiles.end() && it->second == '#') {
        player_.y = oldY;
    }
}

void Roaming::playerMoveLeft() {
    const Room& room = map_.rooms.at(curRoom_);
    if (room.w && player_.x - 1 == -1 && player_.y == room.height / 2) {
        using enum Roaming::Direction;
        goToRoom(room.w.value(), EAST);
        return;
    }
    int32_t oldX = player_.x;
    player_.x = std::clamp(player_.x - 1, 0, map_.rooms.at(curRoom_).width - 1);
    auto it = map_.rooms.at(curRoom_).tiles.find({player_.x, player_.y});
    if (it != map_.rooms.at(curRoom_).tiles.end() && it->second == '#') {
        player_.x = oldX;
    }
}

void Roaming::playerMoveRight() {
    const Room& room = map_.rooms.at(curRoom_);
    if (room.e && player_.x + 1 == room.width && player_.y == room.height / 2) {
        using enum Roaming::Direction;
        goToRoom(room.e.value(), WEST);
        return;
    }
    int32_t oldX = player_.x;
    player_.x = std::clamp(player_.x + 1, 0, map_.rooms.at(curRoom_).width - 1);
    auto it = map_.rooms.at(curRoom_).tiles.find({player_.x, player_.y});
    if (it != map_.rooms.at(curRoom_).tiles.end() && it->second == '#') {
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
    std::optional<ItemID>* place = nullptr;
    switch (itemInfo.wearInfo.value().wearType) {
    case HAND:  place = &player_.hand; break;
    case HEAD:  place = &player_.head; break;
    case TORSO: place = &player_.torso; break;
    case LEGS:  place = &player_.legs; break;
    case FEET:  place = &player_.feet; break;
    }
    assert(place != nullptr);
    if (!place->operator bool()) {
        player_.inventory.erase(player_.inventory.begin() + slot);
        place->operator=(itemID);
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
    auto it = map_.rooms.at(curRoom_).items.find({player_.x, player_.y});
    if (it != map_.rooms.at(curRoom_).items.end()) {
        player_.inventory.push_back(it->second);
        map_.rooms.at(curRoom_).items.erase(it);
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
