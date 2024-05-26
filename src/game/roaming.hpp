#pragma once

#include <esc_menu.hpp>
#include <player.hpp>
#include <state.hpp>
#include <room.hpp>
#include <map.hpp>

class Roaming : public State {
public:
    Roaming(Map map);
    bool init() override;
    State* update(char c) override;
    bool render(Renderer& renderer) override;

private:
    enum class Direction { EAST = 0, NORTH = 1, WEST = 2, SOUTH = 3 };

private:
    void step();

    void playerMoveUp();
    void playerMoveDown();
    void playerMoveLeft();
    void playerMoveRight();

    void playerEquip();
    void playerUnequip();

    void playerPickup();

    void goToRoom(int32_t roomID, Direction fromDir);

    void inventorySelectionUp();
    void inventorySelectionDown();

private:
    EscMenu* escMenu_ = nullptr;
    Player player_;
    Map map_;
    int32_t curRoom_ = 0;
    std::optional<Direction> playerMove_ = std::nullopt;
};
