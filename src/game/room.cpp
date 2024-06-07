#include <utils/random.hpp>
#include <game/room.hpp>
#include <game/item.hpp>
#include <game/tile.hpp>

#include <cassert>
#include <utility>
#include <random>

int32_t Room::getWidth() const {
    return width_;
}

int32_t Room::getHeight() const {
    return height_;
}

int32_t Room::getPlayerX() const {
    return playerX_;
}

int32_t Room::getPlayerY() const {
    return playerY_;
}

void Room::setPlayerX(int32_t x) {
    playerX_ = x;
}

void Room::setPlayerY(int32_t y) {
    playerY_ = y;
}

bool Room::isDoor(int32_t x, int32_t y) const {
    return (x == width_     && y == height_ / 2) // EAST
        || (x == width_ / 2 && y == -1)          // NORTH
        || (x == -1         && y == height_ / 2) // WEST
        || (x == width_ / 2 && y == height_);    // SOUTH
}

bool Room::isPassable(int32_t x, int32_t y) const {
    if (
        x < 0 || x >= width_ ||
        y < 0 || y >= height_
    ) {
        return false;
    }
    auto it = tiles_.find({x,y});
    return it == tiles_.end() || !getTilesDict().at(it->second).isWall;
}

void Room::generate(int32_t width, int32_t height, MobFactory& mobFactory) {
    assert(width >= 0);
    assert(height >= 0);
    assert(width % 2 == 1);
    assert(height % 2 == 1);

    width_ = width;
    height_ = height;

    // tiles
    const auto& tilesDict = getTilesDict();
    const std::vector<TileID> iToTileID = [&](){
        std::vector<TileID> iToTileID_;
        iToTileID_.reserve(tilesDict.size());
        for (const auto& [id, _] : tilesDict) {
            iToTileID_.push_back(id);
        }
        return iToTileID_;
    }();
    std::discrete_distribution tileSpawnDis({1.0, 0.05});
    std::discrete_distribution tileDis = [&](){
        std::vector<double> ws;
        ws.reserve(tilesDict.size());
        for (const auto& [_, v] : tilesDict) {
            ws.push_back(v.w);
        }
        return std::discrete_distribution(ws.begin(), ws.end());
    }();

    // items
    const auto& itemsDict = getItemsDict();
    static const std::vector<int32_t> iToItemID = [&](){
        std::vector<int32_t> iToItemID_;
        iToItemID_.reserve(itemsDict.size());
        for (const auto& [id, _] : itemsDict) {
            iToItemID_.push_back(id);
        }
        return iToItemID_;
    }();
    std::discrete_distribution itemSpawnDis({1.0, 0.01});
    std::discrete_distribution itemDis = [&](){
        std::vector<double> ws;
        ws.reserve(itemsDict.size());
        for (const auto& [_, v] : itemsDict) {
            ws.push_back(v.getW());
        }
        return std::discrete_distribution(ws.begin(), ws.end());
    }();

    // mobs
    std::vector<double> ws = { 1.0, 1.0, 1.0, 1.0 };
    std::discrete_distribution mobSpawnDis({1.0, 0.01});
    std::discrete_distribution mobStratDis(ws.begin(), ws.end());

    auto& rng = getRNG();

    tiles_.clear();
    items_.clear();
    locationToMob_.clear();
    mobs_.clear();

    for (int32_t y = 0; y < height; ++y) {
        for (int32_t x = 0; x < width; ++x) {
            if (x == width / 2 || y == height / 2) {
                // force way between exits
                continue;
            }
            if (tileSpawnDis(rng) == 1) {
                int32_t tileID = iToTileID[tileDis(rng)];
                tiles_[{x,y}] = tileID;
                if (tilesDict.at(tileID).isWall) {
                    continue; // do not add items or mobs over walls
                }
            }

            if (itemSpawnDis(rng) == 1) {
                int32_t itemID = iToItemID[itemDis(rng)];
                items_[{x,y}] = itemID;
            }

            if (mobSpawnDis(rng) == 1) {
                std::unique_ptr<Mob> mob;
                switch (mobStratDis(rng)) {
                case 0:
                    mob.reset(mobFactory.createHostile());
                    break;
                case 1:
                    mob.reset(mobFactory.createPassive());
                    break;
                case 2:
                    mob.reset(mobFactory.createCoward());
                    break;
                case 3:
                    mob.reset(mobFactory.createMold());
                    break;
                default:
                    assert(false);
                    break;
                }
                mob->setX(x);
                mob->setY(y);
                mobs_.push_back(std::move(mob));
                locationToMob_[{x,y}] = mobs_.size() - 1;
            }
        }
    }
}

void Room::generate(
    int32_t minWidth,
    int32_t maxWidth,
    int32_t minHeight,
    int32_t maxHeight,
    MobFactory& mobFactory
) {
    std::uniform_int_distribution widthDis(0, (maxWidth - minWidth) / 2);
    std::uniform_int_distribution heightDis(0, (maxHeight - minHeight) / 2);
    auto& rng = getRNG();
    generate(
        minWidth + 2 * widthDis(rng),
        minHeight + 2 * heightDis(rng),
        mobFactory
    );
}

void Room::generate(MobFactory& mobFactory) {
    static constexpr int32_t MIN_ROOM_WIDTH = 3;
    static constexpr int32_t MAX_ROOM_WIDTH = 21;
    static constexpr int32_t MIN_ROOM_HEIGHT = 3;
    static constexpr int32_t MAX_ROOM_HEIGHT = 21;
    generate(
        MIN_ROOM_WIDTH,
        MAX_ROOM_WIDTH,
        MIN_ROOM_HEIGHT,
        MAX_ROOM_HEIGHT,
        mobFactory
    );
}

bool Room::load(std::istream& in) {
    if (!in) {
        return false;
    }
    tiles_.clear();
    in >> width_ >> height_;

    int32_t nTiles;
    in >> nTiles;
    for (int32_t tileI = 0; tileI < nTiles; ++tileI) {
        int32_t x, y, tileID;
        in >> x >> y >> tileID;
        tiles_[{x,y}] = tileID;
    }
    int32_t nItems;
    in >> nItems;
    for (int32_t itemI = 0; itemI < nItems; ++itemI) {
        int32_t x, y, itemID;
        in >> x >> y >> itemID;
        items_[{x,y}] = itemID;
    }

    return true;
}

const std::map<std::pair<int32_t, int32_t>, int32_t>& Room::getTiles() const {
    return tiles_;
}

const std::map<std::pair<int32_t, int32_t>, int32_t>& Room::getItems() const {
    return items_;
}

std::optional<int32_t> Room::removeItem(int32_t x, int32_t y) {
    auto it = items_.find({x,y});
    if (it == items_.end()) {
        return std::nullopt;
    }
    int32_t res = it->second;
    items_.erase(it);
    return res;
}

void Room::updateMobs() {
    // save initial size to not let mobs spawned
    // during this turn act
    const size_t n = mobs_.size();
    // use indices for iteration here because some
    // mobs might push into mobs vector and invalidate
    // iterators
    for (size_t i = 0; i < n; ++i) {
        mobs_[i]->executeAction(mobs_[i]->pickAction(*this), *this);
    }
}

void Room::relocateMob(
    int32_t fromX, int32_t fromY,
    int32_t toX, int32_t toY
) {
    assert(characterAt(toX, toY) == nullptr);
    assert(isPassable(toX, toY));
    auto it = locationToMob_.find({fromX,fromY});
    if (it == locationToMob_.end()) {
        return;
    }
    int32_t mobID = it->second;
    locationToMob_.erase(it);
    locationToMob_[{toX,toY}] = mobID;
}

Mob& Room::spawnMob(int32_t x, int32_t y, const Mob& prototype) {
    assert(characterAt(x, y) == nullptr);
    assert(isPassable(x, y));
    mobs_.emplace_back(prototype.clone());
    mobs_.back()->setX(x);
    mobs_.back()->setY(y);
    int32_t mobID = ssize(mobs_) - 1;
    locationToMob_[{x,y}] = mobID;
    return *mobs_.back();
}

const std::vector<std::unique_ptr<Mob>>& Room::getMobs() const {
    return mobs_;
}

const Player& Room::player() const {
    assert(player_ != nullptr);
    return *player_;
}

Player& Room::player() {
    assert(player_ != nullptr);
    return *player_;
}

bool Room::hasPlayer() const {
    return player_ != nullptr;
}

void Room::setPlayer(Player* player) {
    player_ = player;
}

const Character* Room::characterAt(int32_t x, int32_t y) const {
    if (x == playerX_ && y == playerY_) {
        return player_;
    }
    auto it = locationToMob_.find({x,y});
    if (it == locationToMob_.end()) {
        return nullptr;
    }
    const auto& mob = mobs_[it->second];
    if (mob->isDead()) {
        return nullptr;
    }
    return mob.get();
}

Character* Room::characterAt(int32_t x, int32_t y) {
    return const_cast<Character*>(
        static_cast<const Room*>(this)->characterAt(x, y)
    );
}
