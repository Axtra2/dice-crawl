#include <game/program_mode/game.hpp>
#include <platform/renderer.hpp>
#include <game/direction.hpp>
#include <game/room.hpp>

#include <cassert>
#include <utility>

void Game::init(Program& program) {
    player_ = Player();
    Room& room = map_.currentRoom();
    room.setPlayerX(room.getWidth() / 2);
    room.setPlayerY(room.getHeight() - 1);
    room.setPlayer(&player_);
    selectedInventorySlot_ = 0;
}

void Game::processInput(Program& program, char c) {
    switch (c) {

    // do not cause simulation step
    case 'j':
        lastCommand_.reset(new MoveInventorySelectionCommand(
            selectedInventorySlot_,
            1
        ));
        return;
    case 'k':
        lastCommand_.reset(new MoveInventorySelectionCommand(
            selectedInventorySlot_,
            -1
        ));
        break;
    case 'q':
        lastCommand_.reset(new EscCommand(program));
        return;

    // cause simulation step
    using enum Direction;
    case 'w':
    case 'a':
    case 's':
    case 'd': {
        static const std::unordered_map<char, Direction> charToDir = {
            { 'w', NORTH },
            { 'a', WEST },
            { 's', SOUTH },
            { 'd', EAST }
        };
        Direction dir = charToDir.at(c);
        lastCommand_.reset(new MoveCommand(map_, player_, dir));
        break;
    }
    case 'p':
        lastCommand_.reset(new PickUpCommand(map_, player_));
        break;
    case 'e':
        lastCommand_.reset(new UseItemCommand(
            map_,
            player_,
            selectedInventorySlot_
        ));
        break;
    default:
        return;
    }

    lastCommand_.reset(new UpdateOnSuccessCommand(
        map_,
        player_,
        program,
        std::move(lastCommand_)
    ));
}

void Game::update(Program& program) {
    if (lastCommand_) {
        lastCommand_->operator()();
        lastCommand_.reset();
    }
}

void Game::render(Program& program, Renderer& renderer) {
    renderer.render(map_, player_, selectedInventorySlot_);
}

void Game::setMap(Map map) {
    map_ = std::move(map);
}
