#include <files.hpp>
#include <room.hpp>
#include <map.hpp>

#include <cassert>
#include <sstream>

std::optional<Map> loadMap(const char* filename) {
    auto data = getFileContents(filename);
    if (!data) {
        return std::nullopt;
    }
    std::stringstream ss(data.value());

    int32_t nRooms;
    ss >> nRooms;

    Map map = { .rooms = std::vector<Room>(nRooms) };

    for (int32_t roomI = 0; roomI < nRooms; ++roomI) {
        Room& room = map.rooms[roomI];
        ss >> room.width >> room.height;

        int32_t nTiles;
        ss >> nTiles;
        for (int32_t tileI = 0; tileI < nTiles; ++tileI) {
            int32_t x, y, tileID;
            ss >> x >> y >> tileID;
            room.tiles[{x,y}] = tileID;
        }
        int32_t nItems;
        ss >> nItems;
        for (int32_t itemI = 0; itemI < nItems; ++itemI) {
            int32_t x, y, itemID;
            ss >> x >> y >> itemID;
            room.items[{x,y}] = itemID;
        }

        char c;
        int32_t e, n, w, s;
        ss >> c >> e;
        assert(c == 'e');
        ss >> c >> n;
        assert(c == 'n');
        ss >> c >> w;
        assert(c == 'w');
        ss >> c >> s;
        assert(c == 's');
        if (e != -1) {
            room.e = e;
        }
        if (n != -1) {
            room.n = n;
        }
        if (w != -1) {
            room.w = w;
        }
    }
    return map;
}
