#include <game/character/mob/strategy/passive.hpp>

MobStrategy::Action Passive::pickAction(const Mob&, const Room&) {
    return {};
}

Passive::Passive()
  : MobStrategy("Passive")
{ }
