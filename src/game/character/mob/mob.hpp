#pragma once

#include <game/character/character.hpp>
#include <game/direction.hpp>
#include <game/color.hpp>

#include <cstdint>
#include <variant>
#include <memory>

class Player;
class Room;
class Map;
class Mob;

class MobStrategy {
public:
    enum class ActionType { NONE = 0, MOVE = 1, REPLICATE = 2 };
    struct Action {
        ActionType type = ActionType::NONE;
        std::unique_ptr<MobStrategy> newStrategy;
        std::variant<std::monostate, Direction> data;
    };

public:
    MobStrategy(std::string_view name);
    ~MobStrategy() = default;
    const std::string& getName() const;
    [[nodiscard]] virtual MobStrategy* clone() const = 0;

private:
    friend class Mob;

    // only Mob can call this
    virtual Action pickAction(const Mob& mob, const Room& room) = 0;

private:
    std::string name_;
};


class Mob : public Character {
public:
    using Action = MobStrategy::Action;
    Mob();

    void receiveAttack(int32_t damage) override;
    virtual Color getColor() const = 0;
    [[nodiscard]] virtual Mob* clone() const = 0;

    virtual Action pickAction(const Room& room);
    virtual void executeAction(Action action, Room& room);
    virtual const std::string& getStrategyName() const;
    virtual void move(Room& room, Direction direction);
    virtual void replicate(Room& room, Direction direction);
    virtual int32_t getX() const;
    virtual int32_t getY() const;
    virtual void setX(int32_t x);
    virtual void setY(int32_t y);
    virtual void setStrategy(std::unique_ptr<MobStrategy> strategy);
    virtual const std::unique_ptr<MobStrategy>& getStrategy() const;

private:
    int32_t x_ = 0;
    int32_t y_ = 0;
    std::unique_ptr<MobStrategy> strategy_;
};


class MobFactory {
public:
    [[nodiscard]] virtual Mob* createHostile() = 0;
    [[nodiscard]] virtual Mob* createPassive() = 0;
    [[nodiscard]] virtual Mob* createCoward() = 0;
    [[nodiscard]] virtual Mob* createMold() = 0;
    virtual ~MobFactory() = default;
};
