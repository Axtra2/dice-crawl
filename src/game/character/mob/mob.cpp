#include <game/character/mob/hostile.hpp>
#include <game/character/mob/mob.hpp>
#include <game/character/player.hpp>
#include <game/direction.hpp>
#include <game/dice.hpp>
#include <game/room.hpp>
#include <utils/inrange.hpp>

Mob::Mob() {
    xp_ = 1;
}

MobStrategy::MobStrategy(std::string_view name)
  : name_(name)
{ }

const std::string& MobStrategy::getName() const {
    return name_;
}

void Mob::receiveAttack(int32_t damage) {
    addInRange(health_, -damage, 0, maxHealth_);
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
    case MOVE:
        move(room, std::get<Direction>(action.data));
        break;
    default:
        break;
    }
    if (action.newStrategy) {
        strategy_ = action.newStrategy;
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
        int32_t attack = throwDice(baseAttackDice_);
        otherCharacter->receiveAttack(attack);
        if (otherCharacter->isDead()) {
            xp_ += otherCharacter->getXP();
        }
        return;
    }
    room.relocateMob(x_, y_, targetX, targetY);
    x_ = targetX;
    y_ = targetY;
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

void Mob::setStrategy(std::shared_ptr<MobStrategy> strategy) {
    strategy_ = strategy;
}

std::shared_ptr<MobStrategy> MobStrategy::genMobStrat() {
    return std::shared_ptr<MobStrategy>(new Hostile());
}
