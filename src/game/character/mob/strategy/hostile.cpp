#include <game/character/mob/strategy/hostile.hpp>
#include <game/room.hpp>

MobStrategy::Action Hostile::pickAction(const Mob& mob, const Room& room) {
    Action action;
    action.type = MobStrategy::ActionType::MOVE;
    int32_t dx = room.getPlayerX() - mob.getX();
    int32_t dy = room.getPlayerY() - mob.getY();
    action.data = dirFromDXDY(dx, dy);
    return action;
}

[[nodiscard]] MobStrategy* Hostile::clone() const {
    return new Hostile();
}

Hostile::Hostile()
  : MobStrategy("Hostile")
{ }
