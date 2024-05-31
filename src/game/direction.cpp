#include <game/direction.hpp>

#include <cassert>
#include <cmath>

Direction dirFromDXDY(int32_t dx, int32_t dy) {
    using enum Direction;
    if (std::abs(dx) > std::abs(dy)) {
        if (dx > 0) {
            return EAST;
        } else {
            return WEST;
        }
    } else {
        if (dy > 0) {
            return SOUTH;
        } else {
            return NORTH;
        }
    }
}
