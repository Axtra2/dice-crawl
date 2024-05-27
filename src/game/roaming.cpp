#include <main_menu.hpp>
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
    if (player_.isDead) {
        // TODO: this is wrong and very temporary
        auto res = new MainMenu();
        res->init();
        return res;
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

void Roaming::step() {
    // update player position
    Room& room = map_.rooms[curRoom_];
    if (playerMove_) {
        using enum Direction;
        int32_t targetX = player_.x;
        int32_t targetY = player_.y;
        switch (playerMove_.value()) {
        case EAST: ++targetX; break;
        case NORTH: --targetY; break;
        case WEST: --targetX; break;
        case SOUTH: ++targetY; break;
        }
        playerMove_ = std::nullopt;
        if (targetX < 0 || targetX >= room.width ||
            targetY < 0 || targetY >= room.height
        ) {
            if (room.e && targetX == room.width && targetY == room.height / 2) {
                using enum Roaming::Direction;
                goToRoom(room.e.value(), WEST);
                return;
            }
            if (room.n && targetX == room.width / 2 && targetY == -1) {
                using enum Roaming::Direction;
                goToRoom(room.n.value(), SOUTH);
                return;
            }
            if (room.w && targetX == -1 && targetY == room.height / 2) {
                using enum Roaming::Direction;
                goToRoom(room.w.value(), EAST);
                return;
            }
            if (room.s && targetX == room.width / 2 && targetY == room.height) {
                using enum Roaming::Direction;
                goToRoom(room.s.value(), NORTH);
                return;
            }
            goto afterPlayerMove;
        }
        auto tileIt = room.tiles.find({targetX, targetY});
        if (tileIt == room.tiles.end() || tileIt->second != '#') {
            auto mobIt = room.locationToMob.find({targetX, targetY});
            if (mobIt == room.locationToMob.end()) {
                player_.x = targetX;
                player_.y = targetY;
            } else {
                Mob& mob = room.mobs[mobIt->second];
                // TODO: put this into separate function
                mob.health = std::max(mob.health - player_.throwDiceForAttack(), 0);
                if (mob.health == 0) {
                    mob.isDead = true;
                    room.locationToMob.erase(mobIt);
                }
            }
        }
    }
afterPlayerMove:
    for (auto& [mobID, mob] : room.mobs) {
        if (mob.isDead) {
            continue;
        }
        auto action = mob.pickAction(room, player_);
        using enum Mob::ActionType;
        switch (action.type) {
        case MOVE: {
            using enum Mob::Direction;
            int32_t targetX = mob.x;
            int32_t targetY = mob.y;
            switch (std::get<Mob::Direction>(action.data)) {
            case EAST: ++targetX; break;
            case NORTH: --targetY; break;
            case WEST: --targetX; break;
            case SOUTH: ++targetY; break;
            }
            if (targetX < 0 || targetX >= room.width ||
                targetY < 0 || targetY >= room.height
            ) {
                goto afterMobAction;
            }
            auto tileIt = room.tiles.find({targetX, targetY});
            if (tileIt == room.tiles.end() || tileIt->second != '#') {
                auto mobIt = room.locationToMob.find({targetX, targetY});
                if (mobIt == room.locationToMob.end() || room.mobs[mobIt->second].isDead) {
                    if (targetX != player_.x || targetY != player_.y) {
                        room.locationToMob.erase({mob.x, mob.y});
                        room.locationToMob.insert({{targetX, targetY}, mobID});
                        mob.x = targetX;
                        mob.y = targetY;
                    } else {
                        player_.health = std::max(player_.health - std::max((mob.calcAttack() - player_.throwDiceForDefense()), 0), 0);
                        if (player_.health == 0) {
                            player_.isDead = true;
                        }
                    }
                } else {
                    int32_t mobID = mobIt->second;
                    Mob& otherMob = room.mobs[mobID];
                    // TODO: put this into separate function
                    otherMob.health = std::max(otherMob.health - mob.calcAttack(), 0);
                    if (otherMob.health == 0) {
                        otherMob.isDead = true;
                        room.locationToMob.erase(mobIt);
                    }
                }
            }
            break;
        }
        case PICKUP:
            break;
        case EQUIP:
            break;
        }
    afterMobAction:
        mob.behaviour = action.newBehaviour;
        continue;
    }
    return;
}

void Roaming::playerMoveUp() {
    playerMove_ = Direction::NORTH;
    step();
}

void Roaming::playerMoveDown() {
    playerMove_ = Direction::SOUTH;
    step();
}

void Roaming::playerMoveLeft() {
    playerMove_ = Direction::WEST;
    step();
}

void Roaming::playerMoveRight() {
    playerMove_ = Direction::EAST;
    step();
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
