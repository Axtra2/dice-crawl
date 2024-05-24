#include <main_menu.hpp>
#include <renderer.hpp>
#include <roaming.hpp>
#include <state.hpp>

#include <algorithm>

bool MainMenu::init() {
    optionNames_ = {
        "New Game",
        "Exit"
    };
    optionActions_ = {
        [](){
            auto res = new Roaming();
            res->init();
            return res;
        },
        [](){return nullptr;}
    };
    return true;
}

State* MainMenu::update(char c) {
    switch (c) {
    case 's':
        ++selectedOption_;
        break;
    case 'w':
        --selectedOption_;
        break;
    case 'e':
        return optionActions_[selectedOption_]();
    }
    selectedOption_ = std::clamp(
        selectedOption_,
        0,
        static_cast<int32_t>(optionNames_.size())
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
