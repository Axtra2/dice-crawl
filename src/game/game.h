#pragma once

#include <unordered_map>
#include <functional>
#include <iostream>
#include <optional>
#include <cassert>
#include <cstdint>
#include <random>
#include <string>
#include <vector>


// aliases

using Health = int32_t;
using Attack = int32_t;
using ItemID = int32_t;
using TileID = char;


// forward declarations

struct Player;
struct ItemInfo;


// player

struct Player {
    int32_t x = 0;
    int32_t y = 0;

    Attack attack = 0;
    Health health = 0;
    Health maxHealth = 0;

    int32_t maxInventorySize = 0;
    std::vector<ItemID> inventory = {};

    std::optional<ItemID> hand = std::nullopt;
    std::optional<ItemID> head = std::nullopt;
    std::optional<ItemID> torso = std::nullopt;
    std::optional<ItemID> legs = std::nullopt;
    std::optional<ItemID> feet = std::nullopt;
};


// items

struct ItemInfo {
    struct WearInfo {
        enum class WearType {
            HAND,
            HEAD,
            TORSO,
            LEGS,
            FEET
        };
        WearType wearType;
        std::optional<std::function<Health(Health)>> maxHealthModifier = std::nullopt;
        std::optional<std::function<Attack(Attack)>> attackModifier = std::nullopt;
    };

    ItemID id = 0;
    WearInfo wearInfo;
};

const auto& itemsDict() {
    static const std::unordered_map<ItemID, ItemInfo> infos_;
    return infos_;
}


// map

class MapGeometry {
public:
    MapGeometry(int32_t width, int32_t height)
      : width_(width),
        height_(height),
        data_(height, std::vector<TileID>(width))
    {
        assert(width >= 0);
        assert(height >= 0);
    }

    const TileID& operator()(int32_t x, int32_t y) const {
        assert(x >= 0);
        assert(y >= 0);
        assert(x < width_);
        assert(y < height_);
        return data_[y][x];
    }

    TileID& operator()(int32_t x, int32_t y) {
        return const_cast<TileID&>(
            static_cast<const MapGeometry*>(this)->operator()(x, y)
        );
    }

    int32_t width() const {
        return width_;
    }

    int32_t height() const {
        return height_;
    }

private:
    int32_t width_ = 0;
    int32_t height_ = 0;
    std::vector<std::vector<TileID>> data_ = {};
};

struct TileInfo {
    TileID id = 0;
    double w = 0;
};

const auto& getTilesDict() {
    static const std::unordered_map<TileID, TileInfo> infos_ = {
        { '.', { .id = '.', .w = 1.0 } },
        { '#', { .id = '#', .w = 0.1 } },
    };
    return infos_;
}

template<typename RNG>
TileID generateRandomTile(RNG& rng) {
    static const std::vector<TileID> iToTileID_ = [](){
        auto& dict = getTilesDict();
        std::vector<TileID> iToTileID;
        iToTileID.reserve(dict.size());
        for (const auto& [id, _] : dict) {
            iToTileID.push_back(id);
        }
        return iToTileID;
    }();
    static std::discrete_distribution dis_ = [](){
        auto& dict = getTilesDict();
        std::vector<double> ws;
        ws.reserve(dict.size());
        for (const auto& [k, v] : dict) {
            ws.push_back(v.w);
        }
        return std::discrete_distribution(ws.begin(), ws.end());
    }();
    return iToTileID_.at(dis_(rng));
}

template<typename RNG>
MapGeometry generateMapGeometry(int32_t width, int32_t height, RNG& rng) {
    assert(width >= 0);
    assert(height >= 0);
    MapGeometry mg(width, height);
    for (decltype(height) y = 0; y < height; ++y) {
        for (decltype(width) x = 0; x < width; ++x) {
            mg(x, y) = generateRandomTile(rng);
        }
    }
    return mg;
}

MapGeometry loadMapGeometry(std::istream& in) {
    return MapGeometry(0, 0);
}
