#include <game/character/mob/confused.hpp>

#include <utility>

Confused::Confused(std::unique_ptr<Mob> wrapped)
  : MobDecorator(std::move(wrapped))
{ }

Color Confused::getColor() const {
    if (lifetime <= 0) {
        return MobDecorator::getColor();
    }
    return { .r = 0x00, .g = 0xFF, .b = 0x00 };
}

void Confused::executeAction(Action action, Room& room) {
    MobDecorator::executeAction(std::move(action), room);
    if (lifetime > 0) {
        --lifetime;
    }
}

Mob::Action Confused::pickAction(const Room& room) {
    if (lifetime <= 0) {
        return MobDecorator::pickAction(room);
    }
    std::uniform_int_distribution dirDis(
        0,
        static_cast<int32_t>(Direction::NDIRS) - 1
    );
    Action action;
    action.type = MobStrategy::ActionType::MOVE;
    action.data = static_cast<Direction>(dirDis(getRNG()));
    return action;
}
