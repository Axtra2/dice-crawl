#include <tile.hpp>

const std::unordered_map<TileID, TileInfo>& getTilesDict() {
    static const std::unordered_map<TileID, TileInfo> dict_ = {
        { TileInfo::EMPTY, { .w = 1.0 } },
        { '#', { .w = 0.1 } },
    };
    return dict_;
}
