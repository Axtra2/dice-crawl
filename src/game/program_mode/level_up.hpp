#pragma once

#include <game/program_mode/menu.hpp>

#include <cstdint>
#include <utility>
#include <string>
#include <vector>

class Player;

const std::pair<
    const std::vector<std::string>&,
    const std::vector<std::function<void(Player&)>>&
>
getLevelRewardOptions(int32_t levelReached);

class LevelUp : public Menu {
public:
    LevelUp(Player* player);
    void setPlayer(Player* player);

private:
    Player* player_;

    // for double buffering
    std::vector<std::function<void(Program&)>> optionActionsBuffer_;
};