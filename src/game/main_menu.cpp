#include <main_menu.hpp>
#include <renderer.hpp>
#include <roaming.hpp>
#include <state.hpp>

#include <algorithm>
#include <cassert>

bool MainMenu::init() {
    optionNames_ = {
        "New Game",
        "Load Game",
        "Exit"
    };
    optionActions_ = {
        [](){
            static auto rng = std::mt19937(std::random_device{}());
            Map map = { .rooms = { generateRoom(21, 21, rng) } };
            auto res = new Roaming(std::move(map));
            res->init();
            return res;
        },
        []() -> State* {
            auto map = loadMap("map.txt");
            if (!map) {
                return nullptr;
            }
            auto res = new Roaming(map.value());
            res->init();
            return res;
        },
        [](){return nullptr;}
    };
    return true;
}

State* MainMenu::update(char c) {
    switch (c) {
    case 'j':
        ++selectedOption_;
        break;
    case 'k':
        --selectedOption_;
        break;
    case 'e':
        return optionActions_[selectedOption_]();
    }

    assert(optionNames_.size() > 0);
    selectedOption_ = std::clamp(
        selectedOption_,
        0,
        static_cast<int32_t>(optionNames_.size() - 1)
    );
    return this;
}

bool MainMenu::render(Renderer& renderer) {
    return renderer.render(
        "Main Menu",
        selectedOption_,
        {optionNames_.data(), optionNames_.size()}
    );
}
