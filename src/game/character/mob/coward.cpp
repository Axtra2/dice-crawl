#include <game/character/mob/coward.hpp>
#include <game/room.hpp>

#include <string>

MobStrategy::Action Coward::pickAction(const Mob& mob, const Room& room) {
    Action action;
    action.type = MobStrategy::ActionType::MOVE;
    int32_t dx = room.getPlayerX() - mob.getX();
    int32_t dy = room.getPlayerY() - mob.getY();
    action.data = oppositeDir[static_cast<int32_t>(dirFromDXDY(dx, dy))];
    return action;
}

const std::string& Coward::getStrategyName() const {
    return name_;
}
