#pragma once

#include <game/direction.hpp>
#include <game/room.hpp>

#include <iostream>
#include <optional>
#include <cstddef>
#include <cstdint>
#include <vector>
#include <array>

class Map {
public:
    const Room& currentRoom() const;
    Room& currentRoom();
    bool currentRoomHasNeighbour(Direction direction) const;
    void goToNeighbour(Direction direction);

    bool load(const char* filename);
    bool load(std::istream& in);
    void generate();

private:
    std::vector<Room> rooms_;
    int32_t currentRoom_ = 0;
    size_t num_layers_ = 0;

    using Neighbours = std::array<
        std::optional<int32_t>,
        static_cast<size_t>(Direction::NDIRS)
    >;

    int32_t boss_room_ = -1;
    std::vector<Neighbours> neighbours_;
    std::vector<std::vector<int32_t>> layers_;
};