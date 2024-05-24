#pragma once

#include <tile.hpp>
#include <item.hpp>

#include <cassert>
#include <cstdint>
#include <random>
#include <map>

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

    room.e = &room; // TODO

    return room;
}
