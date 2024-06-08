#include <game/character/mob/mob_decorator.hpp>

class Confused : public MobDecorator {
public:
    Confused(std::unique_ptr<Mob> wrapped);
    Color getColor() const override;
    Action pickAction(const Room& room) override;
    void executeAction(Action action, Room& room) override;
private:
    int32_t lifetime = 10;
};
