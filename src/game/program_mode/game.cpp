#include <game/program_mode/game.hpp>
#include <platform/renderer.hpp>
#include <game/direction.hpp>
#include <utils/inrange.hpp>
#include <game/program.hpp>

#include <cassert>

void Game::init(Program& program) {
    player_ = Player();
    Room& room = map_.currentRoom();
    room.setPlayerX(room.getWidth() / 2);
    room.setPlayerY(room.getHeight() - 1);
    room.setPlayer(&player_);
    selectedInventorySlot_ = 0;
}

void Game::update(Program& program, char c) {
    bool actionSuccess = false;
    switch (c) {
    // do not cause simulation step
    case 'j': incInRange(selectedInventorySlot_, 0, Player::N_EQ_SLOTS + Player::N_INV_SLOTS); return;
    case 'k': decInRange(selectedInventorySlot_, 0, Player::N_EQ_SLOTS + Player::N_INV_SLOTS); return;
    case 'q':
        program.toEscMenu();
        program.init();
        return;

    // cause simulation step
    using enum Direction;
    case 'w': actionSuccess = player_.move(map_, NORTH); break;
    case 'a': actionSuccess = player_.move(map_, WEST); break;
    case 's': actionSuccess = player_.move(map_, SOUTH); break;
    case 'd': actionSuccess = player_.move(map_, EAST); break;
    case 'p': actionSuccess = player_.pickUp(map_.currentRoom()); break;
    case 'e': {
        using enum ItemInfo::WearInfo::WearType;
        int32_t nEqSlots = static_cast<int32_t>(N_WEAR_TYPES);
        if (selectedInventorySlot_ < nEqSlots) {
            actionSuccess = player_.unequip(
                static_cast<Player::EquipmentSlot>(selectedInventorySlot_)
            );
        } else {
            actionSuccess = player_.equip(selectedInventorySlot_ - nEqSlots);
        }
        break;
    }
    default:
        return;
    }
    if (!actionSuccess) {
        return;
    }
    map_.currentRoom().updateMobs();
    if (player_.isDead()) {
        program.toGameOver();
        program.init();
    }
    assert(player_.getLevel() >= 0);
    if (player_.getXP() >= (1ull << player_.getLevel())) {
        program.toLevelUp(player_);
        program.init();
    }
}

void Game::render(Program& program, Renderer& renderer) {
    renderer.render(map_, player_, selectedInventorySlot_);
}

void Game::setMap(Map map) {
    map_ = std::move(map);
}
