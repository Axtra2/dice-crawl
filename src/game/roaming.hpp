#pragma once

#include <player.hpp>
#include <state.hpp>
#include <room.hpp>

class Roaming : public State {
public:
    bool init() override;
    State* update(char c) override;
    bool render(Renderer& renderer) override;

private:
    void playerMoveUp();
    void playerMoveDown();
    void playerMoveLeft();
    void playerMoveRight();

    void playerEquip();
    void playerUnequip();

    void playerPickup();

private:
    State* prevState = nullptr;
    Player player_;
    Room room_;
};
