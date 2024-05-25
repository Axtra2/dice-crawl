#pragma once

#include <state.hpp>

#include <functional>
#include <cstdint>
#include <string>

class EscMenu : public State {
public:
    EscMenu(State* prevState)
      : prevState_(prevState)
    { }
    bool init() override;
    State* update(char c) override;
    bool render(Renderer& renderer) override;

private:
    State* prevState_ = nullptr;
    int32_t selectedOption_ = 0;
    std::vector<std::string> optionNames_;
    std::vector<std::function<State*()>> optionActions_;
};
