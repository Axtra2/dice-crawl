#pragma once

#include <state.hpp>

class MainMenu : public State {
public:
    bool init() override;
    State* update(char c) override;
    bool render(Renderer& renderer) override;
};
