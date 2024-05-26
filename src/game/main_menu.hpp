#pragma once

#include <state.hpp>

#include <functional>
#include <cstdint>
#include <string>

class MainMenu : public State {
public:
    bool init() override;
    State* update(char c) override;
    bool render(Renderer& renderer) override;

private:
    int32_t selectedOption_ = 0;
    std::vector<std::string> optionNames_;
    std::vector<std::function<State*()>> optionActions_;
};
