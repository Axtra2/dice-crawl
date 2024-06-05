#pragma once

#include <game/character/mob/mob.hpp>
#include <game/character/player.hpp>

#include <unordered_map>
#include <iostream>
#include <optional>
#include <cstdint>
#include <memory>
#include <vector>
#include <span>
#include <map>

class Room {
public:
    int32_t getWidth() const;
    int32_t getHeight() const;

    Room() = default;
    Room(const Room&) = delete;
    Room& operator=(const Room&) = delete;
    Room(Room&&) = default;
    Room& operator=(Room&&) = default;

    const Player& player() const;
    Player& player();

    bool hasPlayer() const;
    void setPlayer(Player* player);

    int32_t getPlayerX() const;
    void setPlayerX(int32_t x);

    int32_t getPlayerY() const;
    void setPlayerY(int32_t y);

    bool isDoor(int32_t x, int32_t y) const;
    bool isPassable(int32_t x, int32_t y) const;

    void relocateMob(
        int32_t fromX, int32_t fromY,
        int32_t toX, int32_t toY
    );
    Mob& spawnMob(int32_t x, int32_t y, const Mob& prototype);
    void updateMobs();
    const std::vector<std::unique_ptr<Mob>>& getMobs() const;

    std::optional<int32_t> removeItem(int32_t x, int32_t y);

    void generate(int32_t width, int32_t height, MobFactory& mobFactory);
    void generate(
        int32_t minWidth,
        int32_t maxWidth,
        int32_t minHeight,
        int32_t maxHeight,
        MobFactory& mobFactory
    );
    void generate(MobFactory& mobFactory);
    bool load(std::istream& in);

    const Character* characterAt(int32_t x, int32_t y) const;
    Character* characterAt(int32_t x, int32_t y);

    const std::map<std::pair<int32_t, int32_t>, int32_t>& getTiles() const;
    const std::map<std::pair<int32_t, int32_t>, int32_t>& getItems() const;

private:
    int32_t width_ = 0;
    int32_t height_ = 0;
    std::map<std::pair<int32_t, int32_t>, int32_t> tiles_;
    std::map<std::pair<int32_t, int32_t>, int32_t> items_;

    std::vector<std::unique_ptr<Mob>> mobs_;
    std::map<std::pair<int32_t, int32_t>, int32_t> locationToMob_;

    Player* player_ = nullptr;
    int32_t playerX_ = 0;
    int32_t playerY_ = 0;
};
