#pragma once

#include <unordered_map>
#include <functional>
#include <iostream>
#include <optional>
#include <cassert>
#include <cstdint>
#include <utility>
#include <random>
#include <string>
#include <vector>
#include <tuple>
#include <map>


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
    static constexpr ItemID EMPTY = 0;

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

    // ItemID id = 0;
    double w = 0.0;
    std::string name;
    std::optional<WearInfo> wearInfo = std::nullopt;
};

const auto& getItemsDict() {
    using enum ItemInfo::WearInfo::WearType;
    static const std::unordered_map<ItemID, ItemInfo> infos_ = {
        { ItemInfo::EMPTY, { .w = 1.0 } },
        { 1, {
            .w = 0.01,
            .name = "sword",
            .wearInfo = ItemInfo::WearInfo{
                .wearType = HAND,
                .attackModifier = [](Attack a){return a + 1;}
            }
        } },
        // { 1, { .w = 0.1, .name = "sword" } },
    };
    return infos_;
}


// map

// class SparceMat2D {
// public:
//     SparceMat2D(int32_t width, int32_t height)
//       : width_(width),
//         height_(height),
//         data_()
//     {
//         assert(width > 0);
//         assert(height > 0);
//     }

//     TileID get(int32_t x, int32_t y) const {
//         auto it = data_.find({x, y});
//         return it == data_.end() ? TileInfo::EMPTY : it->second;
//     }

//     void set(int32_t x, int32_t y, TileID tileID) {
//         if (tileID == TileInfo::EMPTY) {
//             data_.erase({x, y});
//         } else {
//             data_[{x, y}] = tileID;
//         }
//     }

    // const TileID& operator()(int32_t x, int32_t y) const {
    //     assert(x >= 0);
    //     assert(y >= 0);
    //     assert(x < width_);
    //     assert(y < height_);
    //     return data_.at(x, y);
    // }

    // TileID& operator()(int32_t x, int32_t y) {
    //     return const_cast<TileID&>(
    //         static_cast<const RoomGeometry*>(this)->operator()(x, y)
    //     );
    // }

//     int32_t width() const {
//         return width_;
//     }

//     int32_t height() const {
//         return height_;
//     }

// private:
//     int32_t width_ = 0;
//     int32_t height_ = 0;
//     std::map<std::pair<int32_t, int32_t>, TileID> data_;
// };

struct TileInfo {
    static constexpr TileID EMPTY = 0;
    // TileID id = EMPTY;
    double w = 0.0;
};

const auto& getTilesDict() {
    static const std::unordered_map<TileID, TileInfo> infos_ = {
        { TileInfo::EMPTY, { .w = 1.0 } },
        { '#', { .w = 0.1 } },
    };
    return infos_;
}

// template<typename RNG>
// TileID generateRandomTile(
//     std::unordered_map<TileID, double> tileDict,
//     RNG& rng
// ) {
//     static const std::vector<TileID> iToTileID_ = [](){
//         std::vector<TileID> iToTileID;
//         iToTileID.reserve(tileWeightDict.size());
//         for (const auto& [id, _] : dict) {
//             iToTileID.push_back(id);
//         }
//         return iToTileID;
//     }();
//     static std::discrete_distribution dis_ = [](){
//         auto& dict = getTilesDict();
//         std::vector<double> ws;
//         ws.reserve(dict.size());
//         for (const auto& [k, v] : dict) {
//             ws.push_back(v.w);
//         }
//         return std::discrete_distribution(ws.begin(), ws.end());
//     }();
//     return iToTileID_.at(dis_(rng));
// }

// template<typename RNG>
// MapGeometry generateMapGeometry(int32_t width, int32_t height, RNG& rng) {
//     assert(width >= 0);
//     assert(height >= 0);
//     MapGeometry mg(width, height);
//     for (decltype(height) y = 0; y < height; ++y) {
//         for (decltype(width) x = 0; x < width; ++x) {
//             mg(x, y) = generateRandomTile(rng);
//         }
//     }
//     return mg;
// }

// MapGeometry loadMapGeometry(std::istream& in) {
//     return MapGeometry(0, 0);
// }


struct Room {
    int32_t width = 0;
    int32_t height = 0;

    std::map<std::pair<int32_t, int32_t>, TileID> tiles;
    std::map<std::pair<int32_t, int32_t>, ItemID> items;

    // neighbouring rooms
    Room* n = nullptr;
    Room* s = nullptr;
    Room* w = nullptr;
    Room* e = nullptr;
};

// struct Map {
//     std::vector<Room> rooms_;
// };

template<typename RNG>
Room generateRoom(
    int32_t width,
    int32_t height,
    RNG& rng,
    const std::unordered_map<TileID, TileInfo>& tilesDict = getTilesDict(),
    const std::unordered_map<ItemID, ItemInfo>& itemsDict = getItemsDict()
) {
    assert(width >= 0);
    assert(height >= 0);
    assert(width % 2 == 1);
    assert(height % 2 == 1);

    Room room;
    room.width = width;
    room.height = height;

    // tiles
    const std::vector<TileID> iToTileID = [&](){
        std::vector<TileID> iToTileID;
        iToTileID.reserve(tilesDict.size());
        for (const auto& [id, _] : tilesDict) {
            iToTileID.push_back(id);
        }
        return iToTileID;
    }();
    std::discrete_distribution tileDis = [&](){
        std::vector<double> ws;
        ws.reserve(tilesDict.size());
        for (const auto& [_, v] : tilesDict) {
            ws.push_back(v.w);
        }
        return std::discrete_distribution(ws.begin(), ws.end());
    }();

    // items
    static const std::vector<ItemID> iToItemID = [&](){
        std::vector<ItemID> iToItemID;
        iToItemID.reserve(itemsDict.size());
        for (const auto& [id, _] : itemsDict) {
            iToItemID.push_back(id);
        }
        return iToItemID;
    }();
    std::discrete_distribution itemDis = [&](){
        std::vector<double> ws;
        ws.reserve(itemsDict.size());
        for (const auto& [_, v] : itemsDict) {
            ws.push_back(v.w);
        }
        return std::discrete_distribution(ws.begin(), ws.end());
    }();

    for (int32_t y = 0; y < height; ++y) {
        for (int32_t x = 0; x < width; ++x) {
            TileID tileID = iToTileID[tileDis(rng)];
            if (tileID != TileInfo::EMPTY &&
                x != width / 2 && y != height / 2 // force way between exits
            ) {
                room.tiles[{x,y}] = tileID;
                continue; // do not add items over non-empty tiles
            }

            ItemID itemID = iToItemID[itemDis(rng)];
            if (itemID != ItemInfo::EMPTY) {
                room.items[{x,y}] = itemID;
            }
        }
    }

    room.e = &room;

    return room;
}

// template<typename RNG>
// Map generateMap(
//     int32_t nRooms,
//     int32_t minRoomWidth,
//     int32_t maxRoomWidth,
//     int32_t minRoomHeight,
//     int32_t maxRoomHeight,
//     RNG& rng
// ) {
//     std::uniform_int_distribution<int32_t> wdis(minRoomWidth, maxRoomWidth);
//     std::uniform_int_distribution<int32_t> hdis(minRoomHeight, maxRoomHeight);
//     std::uniform_int_distribution<int32_t> ndis(0, 4);

//     Map map;
//     while(true) {
//         int32_t nNeighbours = ndis(rng);
//         auto room = generateRoom(wdis(rng), hdis(rng), rng)
//         map.rooms_.push_back();
//     }
// }