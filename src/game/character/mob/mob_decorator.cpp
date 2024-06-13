#include <game/character/mob/mob_decorator.hpp>

#include <utility>

MobDecorator::MobDecorator(std::unique_ptr<Mob> wrapped)
  : wrapped_(std::move(wrapped))
{ }

bool MobDecorator::isDead() const {
    return wrapped_->isDead();
}

int32_t MobDecorator::getMaxHealth() const {
    return wrapped_->getMaxHealth();
}

int32_t MobDecorator::getHealth() const {
    return wrapped_->getHealth();
}

void MobDecorator::setMaxHealth(int32_t maxHealth) {
    wrapped_->setMaxHealth(maxHealth);
}

void MobDecorator::setHealth(int32_t health) {
    wrapped_->setHealth(health);
}

const Dice<int32_t>& MobDecorator::getBaseAttackDice() const {
    return wrapped_->getBaseAttackDice();
}

const Dice<int32_t>& MobDecorator::getBaseDefenseDice() const {
    return wrapped_->getBaseDefenseDice();
}

Dice<int32_t>& MobDecorator::getBaseAttackDiceMut() {
    return wrapped_->getBaseAttackDiceMut();
}

Dice<int32_t>& MobDecorator::getBaseDefenseDiceMut() {
    return wrapped_->getBaseDefenseDiceMut();
}

uint64_t MobDecorator::getXP() const {
    return wrapped_->getXP();
}

void MobDecorator::setXP(uint64_t xp) {
    wrapped_->setXP(xp);
}

void MobDecorator::receiveAttack(int32_t damage) {
    wrapped_->receiveAttack(damage);
}

Mob::Action MobDecorator::pickAction(const Room& room) {
    return wrapped_->pickAction(room);
}

void MobDecorator::executeAction(Action action, Room& room) {
    wrapped_->executeAction(std::move(action), room);
}

const std::string& MobDecorator::getStrategyName() const {
    return wrapped_->getStrategyName();
}

void MobDecorator::move(Room& room, Direction direction) {
    wrapped_->move(room, direction);
}

void MobDecorator::replicate(Room& room, Direction direction) {
    wrapped_->replicate(room, direction);
}

int32_t MobDecorator::getX() const {
    return wrapped_->getX();
}

int32_t MobDecorator::getY() const {
    return wrapped_->getY();
}

void MobDecorator::setX(int32_t x) {
    wrapped_->setX(x);
}

void MobDecorator::setY(int32_t y) {
    wrapped_->setY(y);
}

void MobDecorator::setStrategy(std::unique_ptr<MobStrategy> strategy) {
    wrapped_->setStrategy(std::move(strategy));
}

const std::unique_ptr<MobStrategy>& MobDecorator::getStrategy() const {
    return wrapped_->getStrategy();
}

Color MobDecorator::getColor() const {
    return wrapped_->getColor();
}

[[nodiscard]] Mob* MobDecorator::clone() const {
    return wrapped_->clone();
}

const std::unique_ptr<Mob>& MobDecorator::getWrapped() const {
    return wrapped_;
}
