#include <main_menu.hpp>
#include <esc_menu.hpp>
#include <renderer.hpp>
#include <roaming.hpp>
#include <state.hpp>

#include <algorithm>
#include <cassert>

bool EscMenu::init() {
    selectedOption_ = 0;
    optionNames_ = {
        "Back to Game",
        "New Game",
        "To Main Menu",
        "Exit"
    };
    optionActions_ = {
        [&](){return prevState_;},
        [&](){
            static auto rng = std::mt19937(std::random_device{}());
            Map map = std::move(MapGenerator().generate_graph(5));
            auto res = new Roaming(std::move(map));
            res->init();
            return res;
        },
        [](){
            auto res = new MainMenu();
            res->init();
            return res;
        },
        [](){return nullptr; /* TODO: save game */ }
    };
    return true;
}

State* EscMenu::update(char c) {
    switch (c) {
    case 'j':
        ++selectedOption_;
        break;
    case 'k':
        --selectedOption_;
        break;
    case 'e':
        return optionActions_[selectedOption_]();
    case 'q':
        return prevState_;
    }

    assert(optionNames_.size() > 0);
    selectedOption_ = std::clamp(
        selectedOption_,
        0,
        static_cast<int32_t>(optionNames_.size() - 1)
    );
    return this;
}

bool EscMenu::render(Renderer& renderer) {
    return renderer.render(
        "Escape Menu",
        selectedOption_,
        {optionNames_.data(), optionNames_.size()}
    );
}
