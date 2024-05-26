#include <mob.hpp>

#include <random>

Mob::Action Mob::pickAction(const Room& room, const Player& player) {
    static std::mt19937 rng(std::random_device{}()); // TODO: make global random generator
    Action action { .newBehaviour = behaviour };
    using enum Behaviour;
    switch (behaviour) {
    case HOSTILE: {
        using enum ActionType;
        action.type = MOVE;
        int32_t dx = player.x - x;
        int32_t dy = player.y - y;
        Direction dir;
        using enum Direction;
        if (std::abs(dx) > std::abs(dy)) {
            if (dx > 0) {
                dir = EAST;
            } else {
                dir = WEST;
            }
        } else {
            if (dy > 0) {
                dir = SOUTH;
            } else {
                dir = NORTH;
            }
        }
        action.data = dir;
        break;
    }
    case PASSIVE: {
        std::uniform_int_distribution dis(0, 3);
        action.type = static_cast<ActionType>(dis(rng));
        using enum ActionType;
        switch (action.type) {
        case MOVE: {
            std::uniform_int_distribution dis(0, 3);
            action.data = static_cast<Direction>(dis(rng));
            break;
        }
        case PICKUP:
            break;
        case EQUIP:
            action.data = 0;
            break;
        }
        break;
    }
    case COWARD:
        break;
    }
    return action;
}

int32_t Mob::calcAttack() const {
    return 3;
}
