#include <game/tile.hpp>

const std::unordered_map<TileID, TileInfo>& getTilesDict() {
    static const std::unordered_map<TileID, TileInfo> dict_ = {
        { '#', { .w = 0.1, .isWall = true } }
    };
    return dict_;
}
