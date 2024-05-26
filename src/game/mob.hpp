#pragma once

#include <player.hpp>

#include <cstdint>
#include <variant>
#include <array>

struct Room;

struct Mob {
    static constexpr std::array<double, 3> ws = { 1.0, 1.0, 1.0 };
    enum class Behaviour { HOSTILE, PASSIVE, COWARD, EMPTY };

    // TODO: merge with Roaming::Direction
    enum class Direction { EAST = 0, NORTH = 1, WEST = 2, SOUTH = 3 };
    enum class ActionType { NONE = 0, MOVE = 1, PICKUP = 2, EQUIP = 3 };
    struct Action {
        ActionType type = ActionType::NONE;
        Behaviour newBehaviour;
        std::variant<std::monostate, Direction, int32_t> data;
    };

    Action pickAction(const Room& room, const Player& player);
    int32_t calcAttack() const;

    int32_t x = 0;
    int32_t y = 0;
    int32_t attack = 0;
    int32_t health = 10;
    bool isDead = false;
    Behaviour behaviour = Behaviour::HOSTILE;
};