#pragma once
#include <game/character/mob/mob.hpp>

#include <memory>

class MobDecorator : public Mob {
public:
    MobDecorator(std::unique_ptr<Mob> wrapped);

    void receiveAttack(int32_t damage) override;

    bool isDead() const override;

    int32_t getMaxHealth() const override;
    int32_t getHealth() const override;
    void setMaxHealth(int32_t maxHealth) override;
    void setHealth(int32_t health) override;

    const Dice<int32_t>& getBaseAttackDice() const override;
    const Dice<int32_t>& getBaseDefenseDice() const override;
    Dice<int32_t>& getBaseAttackDiceMut() override;
    Dice<int32_t>& getBaseDefenseDiceMut() override;
    uint64_t getXP() const override;
    void setXP(uint64_t xp) override;

    Color getColor() const override;
    [[nodiscard]] virtual Mob* clone() const override;

    Action pickAction(const Room& room) override;
    void executeAction(Action action, Room& room) override;
    const std::string& getStrategyName() const override;
    void move(Room& Room, Direction direction) override;
    void replicate(Room& room, Direction direction) override;
    int32_t getX() const override;
    int32_t getY() const override;
    void setX(int32_t x) override;
    void setY(int32_t y) override;
    void setStrategy(std::unique_ptr<MobStrategy> strategy) override;
    const std::unique_ptr<MobStrategy>& getStrategy() const override;

    const std::unique_ptr<Mob>& getWrapped() const;

private:
    std::unique_ptr<Mob> wrapped_;
};
