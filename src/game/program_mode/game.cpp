#include <game/program_mode/game.hpp>

#include <platform/renderer.hpp>
#include <game/direction.hpp>
#include <utils/inrange.hpp>
#include <game/program.hpp>

void Game::init(Program& program) {
    player_ = Player();
    Room& room = map_.currentRoom();
    room.setPlayerX(room.getWidth() / 2);
    room.setPlayerY(room.getHeight() - 1);
    selectedInventorySlot_ = 0;
}

void Game::update(Program& program, char c) {
    switch (c) {
    using enum Direction;
    case 'w': player_.move(map_, NORTH); break;
    case 'a': player_.move(map_, WEST); break;
    case 's': player_.move(map_, SOUTH); break;
    case 'd': player_.move(map_, EAST); break;
    case 'p': player_.pickUp(map_.currentRoom()); break;
    case 'e': {
        using enum Player::EquipmentSlot;
        int32_t nEqSlots = static_cast<int32_t>(N_WEAR_TYPES);
        if (selectedInventorySlot_ < nEqSlots) {
            player_.unequip(static_cast<Player::EquipmentSlot>(selectedInventorySlot_));
        } else {
            player_.equip(selectedInventorySlot_ - nEqSlots);
        }
        break;
    }
    case 'j': incInRange(selectedInventorySlot_, 0, Player::N_EQ_SLOTS + Player::N_INV_SLOTS); return;
    case 'k': decInRange(selectedInventorySlot_, 0, Player::N_EQ_SLOTS + Player::N_INV_SLOTS); return;
    case 'q':
        program.toEscMenu();
        program.init();
        return;
    default:
        return;
    }
    map_.currentRoom().updateMobs(player_);
    if (player_.isDead()) {
        program.toMainMenu(); // TODO: game over state
        program.init();
    }
}

void Game::render(Program& program, Renderer& renderer) {
    renderer.render(map_, player_, selectedInventorySlot_);
}

void Game::setMap(Map map) {
    map_ = std::move(map);
}
