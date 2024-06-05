#include <game/character/player.hpp>
#include <game/room.hpp>
#include <game/tile.hpp>

#include <sstream>

void testRoom() {
    for (auto& [tileID, tileInfo] : getTilesDict()) {
        if (!tileInfo.isWall) {
            continue;
        }
        std::stringstream ss(R"(
            3 1
            1
            1 0 )" + std::to_string(tileID) + R"(
            1
            0 0 1
        )");
        Room room;
        room.load(ss);

        assert(room.getWidth() == 3);
        assert(room.getHeight() == 1);

        for (int y = 0; y < room.getHeight(); ++y) {
            for (int x = 0; x < room.getWidth(); ++x) {
                assert(!room.isDoor(x, y));
                assert(room.characterAt(x, y) == nullptr);
            }
        }

        assert(room.isDoor(3, 0));
        assert(room.isDoor(1, -1));
        assert(room.isDoor(-1, 0));
        assert(room.isDoor(1, 1));

        assert(!room.isPassable(3, 0));
        assert(!room.isPassable(1, -1));
        assert(!room.isPassable(-1, 0));
        assert(!room.isPassable(1, 1));

        assert(room.isPassable(0, 0));
        assert(!room.isPassable(1, 0));
        assert(room.isPassable(2, 0));

        {
            auto& tiles = room.getTiles();
            assert(tiles.size() == 1);
            auto it = tiles.find({1,0});
            assert(it != room.getTiles().end());
            assert(it->second == tileID);
        }

        {
            auto& items = room.getItems();
            assert(items.size() == 1);
            auto it = items.find({0,0});
            assert(it != items.end());
            assert(it->second == 1);
        }

        {
            auto item = room.removeItem(0, 0);
            assert(item.has_value());
            assert(item.value() == 1);
            assert(room.getItems().size() == 0);
            item = room.removeItem(0, 0);
            assert(!item.has_value());
        }

        {
            Player p;

            assert(!room.hasPlayer());

            room.setPlayer(&p);
            assert(room.hasPlayer());
            assert(&room.player() == &p);

            room.setPlayerX(0);
            room.setPlayerY(0);
            assert(room.getPlayerX() == 0);
            assert(room.getPlayerY() == 0);
            assert(room.characterAt(0, 0) == &p);
        }
    }
}

int main() {
    testRoom();
}
