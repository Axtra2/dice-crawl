#include <game/character/mob/strategy/passive.hpp>

MobStrategy::Action Passive::pickAction(const Mob&, const Room&) {
    return {};
}

[[nodiscard]] MobStrategy* Passive::clone() const {
    return new Passive();
}

Passive::Passive()
  : MobStrategy("Passive")
{ }
