#include <main_menu.hpp>
#include <state.hpp>
#include <game.hpp>

bool Game::init() {
    if (state == nullptr) {
        state = new MainMenu();
    }
    return state->init();
}

bool Game::update(char c) {
    auto newState = state->update(c);
    if (newState == nullptr) {
        return false;
    }
    state = newState;
    return true;
}

bool Game::render(Renderer& renderer) {
    return state->render(renderer);
}
