#pragma once

#include <game/character/character.hpp>
#include <game/direction.hpp>

#include <cstdint>
#include <variant>
#include <memory>

class Player;
class Room;
class Map;
class Mob;

class MobStrategy {
public:
    enum class ActionType { NONE = 0, MOVE = 1 };
    struct Action {
        ActionType type = ActionType::NONE;
        std::shared_ptr<MobStrategy> newStrategy;
        std::variant<std::monostate, Direction> data;
    };

public:
    MobStrategy(std::string_view name);

    virtual Action pickAction(const Mob& mob, const Room& room) = 0;

    const std::string& getName() const;

private:
    std::string name_;
};

class Mob : public Character {
public:
    using Action = MobStrategy::Action;
    Mob();
    void receiveAttack(int32_t damage) override;
    Action pickAction(const Room& room);
    void executeAction(Action action, Room& room);
    const std::string& getStrategyName() const;
    void move(Room& Room, Direction direction);
    int32_t getX() const;
    int32_t getY() const;
    void setX(int32_t x);
    void setY(int32_t y);
    void setStrategy(std::shared_ptr<MobStrategy> strategy);

private:
    int32_t x_ = 0;
    int32_t y_ = 0;
    std::shared_ptr<MobStrategy> strategy_;
};
