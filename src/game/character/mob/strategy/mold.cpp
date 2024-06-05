#include <game/character/mob/strategy/mold.hpp>
#include <game/direction.hpp>
#include <utils/random.hpp>

#include <random>

MobStrategy::Action Mold::pickAction(const Mob&, const Room&) {
    static constexpr double p = 0.1;
    std::discrete_distribution replDis({1.0 - p, p});
    auto& rng = getRNG();
    if (replDis(rng) == 0) {
        return {};
    }

    std::uniform_int_distribution dirDis(
        0,
        static_cast<int32_t>(Direction::NDIRS) - 1
    );
    Action action;
    action.type = ActionType::REPLICATE;
    action.data = static_cast<Direction>(dirDis(rng));
    return action;
}

[[nodiscard]] MobStrategy* Mold::clone() const {
    return new Mold();
}

Mold::Mold()
  : MobStrategy("Mold")
{ }
