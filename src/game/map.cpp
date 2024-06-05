#include <platform/files.hpp>
#include <game/direction.hpp>
#include <game/map.hpp>

#include <algorithm>
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
    auto& initRoom = rooms_[currentRoom_];
    assert(initRoom.hasPlayer());
    auto& player = initRoom.player();
    auto& neighbours = neighbours_[currentRoom_];
    currentRoom_ = neighbours[static_cast<size_t>(direction)].value();
    auto& room = rooms_[currentRoom_];
    room.setPlayerX(room.getWidth() / 2);
    room.setPlayerY(room.getHeight() - 1);
    room.setPlayer(&player);
    initRoom.setPlayer(nullptr);
}

void Map::generate(
    int32_t minNLayers,
    int32_t maxNLayers,
    int32_t minRoomWidth,
    int32_t maxRoomWidth,
    int32_t minRoomHeight,
    int32_t maxRoomHeight
) {
    std::uniform_int_distribution dis(minNLayers, maxNLayers);
    num_layers_ = dis(getRNG());
    layers_.assign(num_layers_ + 1, {});

    generate_node(
        0,
        minRoomWidth,
        maxRoomWidth,
        minRoomHeight,
        maxRoomHeight
    );
    boss_room_ = generate_node(
        num_layers_,
        minRoomWidth,
        maxRoomWidth,
        minRoomHeight,
        maxRoomHeight
    );
    traverse(
        layers_[0][0],
        0,
        minRoomWidth,
        maxRoomWidth,
        minRoomHeight,
        maxRoomHeight
    );
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

    rooms_.clear();
    rooms_.reserve(nRooms);

    neighbours_.assign(nRooms, {});

    for (int32_t roomI = 0; roomI < nRooms; ++roomI) {
        rooms_.push_back(Room());
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

void Map::traverse(
    int32_t node_idx,
    int32_t curr_layer,
    int32_t minRoomWidth,
    int32_t maxRoomWidth,
    int32_t minRoomHeight,
    int32_t maxRoomHeight
) {
    using enum Direction;
    int32_t num_nodes;
    std::vector<int32_t> connections;
    auto& rng = getRNG();
    {
        if (curr_layer + 1 >= num_layers_) {
            neighbours_[node_idx][static_cast<size_t>(NORTH)] = boss_room_;
            return;
        }

        std::uniform_int_distribution<> dis(1, 3);

        num_nodes = dis(rng);

        if (layers_[curr_layer + 1].size() >= 3) {
            std::uniform_real_distribution<> prob(0.0, 1.0);
            double p = 0.4;
            int old_count = std::count_if(layers_[curr_layer + 1].begin(), layers_[curr_layer + 1].end(),
                [&prob, &rng, p](int32_t n) { return prob(rng) > p; });

            if (old_count > num_nodes) old_count = num_nodes;
            std::sample(layers_[curr_layer + 1].begin(), layers_[curr_layer + 1].end(),
                        std::back_inserter(connections), old_count, rng);
            num_nodes -= old_count;
        }
    }
    for (int i = 0; i < num_nodes; ++i) {
        int32_t new_node = generate_node(
            curr_layer + 1,
            minRoomWidth,
            maxRoomWidth,
            minRoomHeight,
            maxRoomHeight
        );
        connections.push_back(new_node);
        traverse(
            new_node,
            curr_layer + 1,
            minRoomWidth,
            maxRoomWidth,
            minRoomHeight,
            maxRoomHeight
        );
    }

    Direction dirs[] = { EAST, NORTH, WEST };
    std::shuffle(std::begin(dirs), std::end(dirs), rng);
    auto& ns = neighbours_[node_idx];
    for (size_t i = 0; i < connections.size(); ++i) {
        ns[static_cast<size_t>(dirs[i])] = connections[i];
    }
}

int32_t Map::generate_node(
    int32_t layer,
    int32_t minWidth,
    int32_t maxWidth,
    int32_t minHeight,
    int32_t maxHeight
) {
    Room room;
    room.generate(
        minWidth,
        maxWidth,
        minHeight,
        maxHeight
    );
    const int32_t id = rooms_.size();
    rooms_.push_back(std::move(room));
    layers_[layer].push_back(id);
    neighbours_.emplace_back();
    return id;
}


MapBuilder& MapBuilder::file(std::string_view filename) {
    assert(!stream_.has_value());
    filename_ = filename;
    return *this;
}

MapBuilder& MapBuilder::stream(std::istream& is) {
    assert(!filename_.has_value());
    stream_ = &is;
    return *this;
}

MapBuilder& MapBuilder::minNLayers(int32_t minNLayers) {
    assert(!filename_.has_value());
    assert(!stream_.has_value());
    minNLayers_ = std::max(0, minNLayers);
    maxNLayers_ = std::max(minNLayers_, maxNLayers_);
    return *this;
}

MapBuilder& MapBuilder::maxNLayers(int32_t maxNLayers) {
    assert(!filename_.has_value());
    assert(!stream_.has_value());
    maxNLayers_ = std::max(0, maxNLayers);
    minNLayers_ = std::min(minNLayers_, maxNLayers_);
    return *this;
}

MapBuilder& MapBuilder::nLayers(int32_t nLayers) {
    assert(!filename_.has_value());
    assert(!stream_.has_value());
    minNLayers(nLayers);
    maxNLayers(nLayers);
    return *this;
}

MapBuilder& MapBuilder::maxRoomSize(int32_t maxWidth, int32_t maxHeight) {
    assert(!filename_.has_value());
    assert(!stream_.has_value());
    maxRoomWidth_ = std::max(0, maxWidth);
    maxRoomHeight_ = std::max(0, maxHeight);

    minRoomWidth_ = std::min(minRoomWidth_, maxRoomWidth_);
    minRoomHeight_ = std::min(minRoomHeight_, maxRoomHeight_);
    return *this;
}

MapBuilder& MapBuilder::minRoomSize(int32_t minWidth, int32_t minHeight) {
    assert(!filename_.has_value());
    assert(!stream_.has_value());
    minRoomWidth_ = std::max(0, minWidth);
    minRoomHeight_ = std::max(0, minHeight);

    maxRoomWidth_ = std::max(minRoomWidth_, maxRoomWidth_);
    maxRoomHeight_ = std::max(minRoomHeight_, maxRoomHeight_);
    return *this;
}

MapBuilder& MapBuilder::roomSize(int32_t width, int32_t height) {
    assert(!filename_.has_value());
    assert(!stream_.has_value());
    minRoomSize(width, height);
    maxRoomSize(width, height);
    return *this;
}

Map MapBuilder::build() {
    Map map;
    if (filename_) {
        map.load(filename_.value().c_str());
    } else if (stream_) {
        map.load(*stream_.value());
    } else {
        map.generate(
            minNLayers_,
            maxNLayers_,
            minRoomWidth_,
            maxRoomWidth_,
            minRoomHeight_,
            maxRoomHeight_
        );
    }
    return map;
}
