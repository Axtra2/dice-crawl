#include <platform/files.hpp>
#include <game/direction.hpp>
#include <game/map.hpp>

#include <optional>
#include <cassert>
#include <cstddef>
#include <sstream>

const Room& Map::currentRoom() const {
    assert(rooms_.size() > 0);
    return rooms_[currentRoom_];
}

Room& Map::currentRoom() {
    assert(rooms_.size() > 0);
    return rooms_[currentRoom_];
}

bool Map::currentRoomHasNeighbour(Direction direction) const {
    auto& neighbours = neighbours_[currentRoom_];
    return neighbours[static_cast<size_t>(direction)].has_value();
}

void Map::goToNeighbour(Direction direction) {
    assert(currentRoomHasNeighbour(direction));
    auto& neighbours = neighbours_[currentRoom_];
    currentRoom_ = neighbours[static_cast<size_t>(direction)].value();
    auto& room = rooms_[currentRoom_];
    room.setPlayerX(room.getWidth() / 2);
    room.setPlayerY(room.getHeight() - 1);
}

void Map::generate() {
    rooms_.clear();
    Room room;
    room.generate();
    rooms_.push_back(std::move(room));
    neighbours_.assign(1, {});
}

bool Map::load(const char* filename) {
    auto data = getFileContents(filename);
    if (!data) {
        return false;
    }
    std::stringstream ss = std::stringstream(data.value());
    return load(ss);
}

bool Map::load(std::istream& in) {
    if (!in) {
        return false;
    }
    int32_t nRooms;
    in >> nRooms;

    rooms_.assign(nRooms, Room());
    neighbours_.assign(nRooms, {});

    for (int32_t roomI = 0; roomI < nRooms; ++roomI) {
        rooms_[roomI].load(in);

        char c;
        int32_t e, n, w, s;
        in >> c >> e;
        assert(c == 'e');
        in >> c >> n;
        assert(c == 'n');
        in >> c >> w;
        assert(c == 'w');
        in >> c >> s;
        assert(c == 's');

        using enum Direction;
        if (e != -1) {
            neighbours_[roomI][static_cast<size_t>(EAST)] = e;
        }
        if (n != -1) {
            neighbours_[roomI][static_cast<size_t>(NORTH)] = n;
        }
        if (w != -1) {
            neighbours_[roomI][static_cast<size_t>(WEST)] = w;
        }
        if (s != -1) {
            neighbours_[roomI][static_cast<size_t>(SOUTH)] = s;
        }
    }
    return true;
}
