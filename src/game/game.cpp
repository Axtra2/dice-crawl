#include <roaming.hpp>
#include <state.hpp>
#include <game.hpp>

bool Game::init() {
    if (state == nullptr) {
        state = new Roaming();
    }
    return state->init();
}

bool Game::update(char c) {
    state = state->update(c);
    return state != nullptr;
}

bool Game::render(Renderer& renderer) {
    return state->render(renderer);
}
