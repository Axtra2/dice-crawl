#include <game/character/mob/passive.hpp>
#include <utils/random.hpp>

#include <random>
#include <string>

MobStrategy::Action Passive::pickAction(const Mob&, const Room&) {
    Action action;
    action.type = MobStrategy::ActionType::MOVE;

    std::uniform_int_distribution<int32_t> dirDist(
        0,
        static_cast<int32_t>(Direction::NDIRS)
    );

    auto& rng = getRNG();
    action.data = static_cast<Direction>(dirDist(rng));
    return action;
}

const std::string& Passive::getStrategyName() const {
    return name_;
}
