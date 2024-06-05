#include <game/direction.hpp>
#include <game/map.hpp>

#include <cassert>

void testMap() {
    Map map;
    map.load("assets/map.txt");

    Player p;
    map.currentRoom().setPlayer(&p);

    using enum Direction;

    assert(map.currentRoom().hasPlayer());
    assert(map.currentRoom().getWidth() == 50);
    assert(map.currentRoom().getHeight() == 50);
    assert(map.currentRoomHasNeighbour(EAST));
    assert(map.currentRoomHasNeighbour(NORTH));
    assert(!map.currentRoomHasNeighbour(WEST));
    assert(!map.currentRoomHasNeighbour(SOUTH));

    map.goToNeighbour(NORTH);
    assert(map.currentRoom().hasPlayer());
    assert(map.currentRoom().getWidth() == 3);
    assert(map.currentRoom().getHeight() == 3);
    assert(map.currentRoomHasNeighbour(EAST));
    assert(!map.currentRoomHasNeighbour(NORTH));
    assert(!map.currentRoomHasNeighbour(WEST));
    assert(map.currentRoomHasNeighbour(SOUTH));

    map.goToNeighbour(EAST);
    assert(map.currentRoom().hasPlayer());
    assert(map.currentRoom().getWidth() == 9);
    assert(map.currentRoom().getHeight() == 9);
    assert(!map.currentRoomHasNeighbour(EAST));
    assert(!map.currentRoomHasNeighbour(NORTH));
    assert(map.currentRoomHasNeighbour(WEST));
    assert(map.currentRoomHasNeighbour(SOUTH));

    map.goToNeighbour(SOUTH);
    assert(map.currentRoom().hasPlayer());
    assert(map.currentRoom().getWidth() == 7);
    assert(map.currentRoom().getHeight() == 7);
    assert(!map.currentRoomHasNeighbour(EAST));
    assert(map.currentRoomHasNeighbour(NORTH));
    assert(map.currentRoomHasNeighbour(WEST));
    assert(!map.currentRoomHasNeighbour(SOUTH));

    map.goToNeighbour(WEST);
    assert(map.currentRoom().getWidth() == 50);
    assert(map.currentRoom().getHeight() == 50);
}

int main() {
    testMap();
}
