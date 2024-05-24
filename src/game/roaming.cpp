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
    if (player_.inventory.empty()) {
        return;
    }
    ItemID itemID = player_.inventory.front(); // TODO
    const ItemInfo& itemInfo = getItemsDict().at(itemID);
    if (!itemInfo.wearInfo) {
        return;
    }
    using enum ItemInfo::WearInfo::WearType;
    switch (itemInfo.wearInfo.value().wearType) {
    case HAND:
        if (!player_.hand) {
            player_.inventory.erase(player_.inventory.begin());
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
}

void Roaming::playerUnequip() {
    // TODO
}

void Roaming::playerPickup() {
    auto it = room_.items.find({player_.x, player_.y});
    if (it != room_.items.end()) {
        player_.inventory.push_back(it->second);
        room_.items.erase(it);
    }
}
