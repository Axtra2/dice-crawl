#include <game/character/mob/passive.hpp>

MobStrategy::Action Passive::pickAction(const Mob&, const Room&) {
    return {};
}

Passive::Passive()
  : MobStrategy("Passive")
{ }
