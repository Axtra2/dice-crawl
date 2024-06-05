#include <game/character/mob/mob.hpp>
#include <game/character/player.hpp>
#include <game/direction.hpp>
#include <game/dice.hpp>
#include <game/room.hpp>
#include <utils/inrange.hpp>

#include <utility>

Mob::Mob() {
    setXP(1);
}

MobStrategy::MobStrategy(std::string_view name)
  : name_(name)
{ }

const std::string& MobStrategy::getName() const {
    return name_;
}

void Mob::receiveAttack(int32_t damage) {
    int32_t h = getHealth();
    addInRange(h, -damage, 0, getMaxHealth());
    setHealth(h);
}

Mob::Action Mob::pickAction(const Room& room) {
    if (isDead() || !strategy_) {
        return {};
    }
    return strategy_->pickAction(*this, room);
}

void Mob::executeAction(Action action, Room& room) {
    switch (action.type) {
    using enum MobStrategy::ActionType;
    case NONE:
        break;
    case MOVE:
        move(room, std::get<Direction>(action.data));
        break;
    case REPLICATE:
        replicate(room, std::get<Direction>(action.data));
        break;
    }
    if (action.newStrategy) {
        strategy_ = std::move(action.newStrategy);
    }
}

void Mob::move(Room& room, Direction direction) {
    auto [dx, dy] = dirToDXDY[static_cast<size_t>(direction)];
    int32_t targetX = x_ + dx;
    int32_t targetY = y_ + dy;
    if (!room.isPassable(targetX, targetY)) {
        return;
    }
    Character* otherCharacter = room.characterAt(targetX, targetY);
    if (otherCharacter != nullptr) {
        int32_t attack = throwDice(getBaseAttackDice());
        otherCharacter->receiveAttack(attack);
        if (otherCharacter->isDead()) {
            setXP(getXP() + otherCharacter->getXP());
        }
        return;
    }
    room.relocateMob(x_, y_, targetX, targetY);
    x_ = targetX;
    y_ = targetY;
}

void Mob::replicate(Room& room, Direction direction) {
    auto [dx, dy] = dirToDXDY[static_cast<size_t>(direction)];
    int32_t targetX = x_ + dx;
    int32_t targetY = y_ + dy;
    if (!room.isPassable(targetX, targetY)) {
        return;
    }
    Character* otherCharacter = room.characterAt(targetX, targetY);
    if (otherCharacter != nullptr) {
        int32_t attack = throwDice(getBaseAttackDice());
        otherCharacter->receiveAttack(attack);
        if (otherCharacter->isDead()) {
            setXP(getXP() + otherCharacter->getXP());
        }
        return;
    }
    Mob& replica = room.spawnMob(targetX, targetY, *this);
    replica.setXP(0);
}

int32_t Mob::getX() const {
    return x_;
}

int32_t Mob::getY() const {
    return y_;
}

void Mob::setX(int32_t x) {
    x_ = x;
}

void Mob::setY(int32_t y) {
    y_ = y;
}

const std::string& Mob::getStrategyName() const {
    return strategy_->getName();
}

const std::unique_ptr<MobStrategy>& Mob::getStrategy() const {
    return strategy_;
}

void Mob::setStrategy(std::unique_ptr<MobStrategy> strategy) {
    strategy_ = std::move(strategy);
}
