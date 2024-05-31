#include <game/program_mode/main_menu.hpp>
#include <game/program_mode/esc_menu.hpp>
#include <game/program_mode/game.hpp>
#include <game/program.hpp>

#include <cassert>
#include <utility>
#include <memory>

Program::Program() {
    toMainMenu();
}

void Program::init() {
    mode_.lock()->init(*this);
}

void Program::update(char c) {
    mode_.lock()->update(*this, c);
}

void Program::render(Renderer& renderer) {
    mode_.lock()->render(*this, renderer);
}

void Program::toMainMenu() {
    if (!mainMenu_) {
        mainMenu_.reset(new MainMenu);
    }
    mode_ = mainMenu_;
}

void Program::toEscMenu() {
    if (!escMenu_) {
        escMenu_.reset(new EscMenu);
    }
    mode_ = escMenu_;
}

void Program::toGame() {
    if (!game_) {
        game_.reset(new Game);
    }
    mode_ = game_;
}

void Program::toGame(Map map) {
    toGame();
    Game* game = dynamic_cast<Game*>(game_.get());
    assert(game != nullptr);
    game->setMap(std::move(map));
}

void Program::finish() {
    mode_.reset();
}

bool Program::finished() const {
    return static_cast<bool>(mode_.expired());
}
