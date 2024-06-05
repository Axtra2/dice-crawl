#pragma once

#include <game/direction.hpp>
#include <game/room.hpp>

#include <algorithm>
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
    void generate(
        int32_t minNLayers,
        int32_t maxNLayers,
        int32_t minRoomWidth,
        int32_t maxRoomWidth,
        int32_t minRoomHeight,
        int32_t maxRoomHeight,
        MobFactory& mobFactory
    );

private:
    void traverse(
        int32_t node_idx,
        int32_t curr_layer,
        int32_t minRoomWidth,
        int32_t maxRoomWidth,
        int32_t minRoomHeight,
        int32_t maxRoomHeight,
        MobFactory& mobFactory
    );

    int32_t generate_node(
        int32_t layer,
        int32_t minWidth,
        int32_t maxWidth,
        int32_t minHeight,
        int32_t maxHeight,
        MobFactory& mobFactory
    );

private:
    std::vector<Room> rooms_;
    int32_t currentRoom_ = 0;

    using Neighbours = std::array<
        std::optional<int32_t>,
        static_cast<size_t>(Direction::NDIRS)
    >;

    int32_t boss_room_ = -1;
    int32_t num_layers_ = 0;
    std::vector<Neighbours> neighbours_;
    std::vector<std::vector<int32_t>> layers_;
};


class MapBuilder {
public:
    MapBuilder& file(std::string_view filename);
    MapBuilder& stream(std::istream& is);

    MapBuilder& minNLayers(int32_t minNLayers);
    MapBuilder& maxNLayers(int32_t maxNLayers);
    MapBuilder& nLayers(int32_t nLayers);

    MapBuilder& maxRoomSize(int32_t maxWidth, int32_t maxHeight);
    MapBuilder& minRoomSize(int32_t minWidth, int32_t minHeight);
    MapBuilder& roomSize(int32_t width, int32_t height);

    MapBuilder& mobFactory(MobFactory& mobFactory);

    Map build();
private:
    std::optional<std::string> filename_;
    std::optional<std::istream*> stream_;
    int32_t minNLayers_ = 3;
    int32_t maxNLayers_ = 10;
    int32_t minRoomWidth_ = 3;
    int32_t maxRoomWidth_ = 21;
    int32_t minRoomHeight_ = 3;
    int32_t maxRoomHeight_ = 21;
    MobFactory* mobFactory_ = nullptr;
};
