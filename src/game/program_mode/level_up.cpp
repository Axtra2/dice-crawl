#include <game/program_mode/level_up.hpp>
#include <game/character/player.hpp>
#include <game/program.hpp>

#include <cassert>
#include <sstream>

const std::pair<
    const std::vector<std::string>&,
    const std::vector<std::function<void(Player&)>>&
>
getLevelRewardOptions(int32_t levelReached) {
    assert(levelReached >= 1);
    static const std::vector<
        std::vector<std::string>
    > levelRewardNames_ = {
        { // for reaching level 1
            "Max Health + 1",
        }
    };
    static const std::vector<
        std::vector<std::function<void(Player&)>>
    > levelRewardActions_ = {
        { // for reaching level 1
            [](Player& player){ player.setMaxHealth(player.getMaxHealth() + 1); }
        }
    };
    assert(levelRewardNames_.size() == levelRewardActions_.size());
    assert(levelRewardNames_.size() > 0);
    int32_t i = levelReached - 1;
    if (i >= levelRewardNames_.size()) {
        i = levelRewardNames_.size() - 1;
    }
    return {
        levelRewardNames_[i],
        levelRewardActions_[i]
    };
}

LevelUp::LevelUp(Player* player)
  : Menu("", {""}, {[](auto&){}})
{
    setPlayer(player);
}

void LevelUp::setPlayer(Player* player) {
    player_ = player;
    if (player_) {
        auto [names, actions] = getLevelRewardOptions(player_->getLevel() + 1);
        std::stringstream titleSS;
        titleSS << "You've reached level "
                << std::to_string(player_->getLevel() + 1)
                << "!"
                << " Choose one reward:";
        name_ = titleSS.str();
        optionNames_ = names;
        optionActionsBuffer_.clear();
        assert(actions.size() == names.size());
        optionActionsBuffer_.reserve(actions.size());
        for (auto& action : actions) {
            optionActionsBuffer_.push_back(
                [&](Program& program){
                    assert(player_);
                    player_->levelUp();
                    action(*player_);
                    player_->setHealth(player_->getMaxHealth());
                    if (player_->canLevelUp()) {
                        program.toLevelUp(*player_);
                    } else {
                        program.toGame();
                    }
                }
            );
        }
        optionActions_.swap(optionActionsBuffer_);
    }
}
