#include <game/program_mode/menu.hpp>
#include <platform/renderer.hpp>
#include <utils/inrange.hpp>

#include <cassert>
#include <utility>

Menu::Menu(
    std::string name,
    std::vector<std::string> optionNames,
    std::vector<std::function<void(Program&)>> optionActions
) : name_(std::move(name)),
    selectedOption_(0),
    optionNames_(std::move(optionNames)),
    optionActions_(std::move(optionActions))
{
    assert(optionNames_.size() == optionActions_.size());
    assert(optionActions_.size() > 0);
}

void Menu::init(Program& program) {
    selectedOption_ = 0;
}

void Menu::update(Program& program, char c) {
    switch (c) {
    case 'j':
        incInRange(selectedOption_, 0, optionActions_.size());
        break;
    case 'k':
        decInRange(selectedOption_, 0, optionActions_.size());
        break;
    case 'e':
        optionActions_[selectedOption_](program);
    }
}

void Menu::render(Program& program, Renderer& renderer) {
    renderer.render(
        name_,
        selectedOption_,
        {optionNames_.data(), optionNames_.size()}
    );
}
