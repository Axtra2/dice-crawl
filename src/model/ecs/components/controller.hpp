#pragma once

#include <ecs/components/logger.hpp>
#include <ecs/components/transform.hpp>
#include <ecs/gameobject.hpp>
#include <string>

namespace ecs {
class Controller : public Component {
   private:
    Transform* transform;
    Logger* logger;
    char input_;

   public:
    void start() override {
        transform = this->gameObject->getComponent<Transform>();
        logger = this->gameObject->getComponent<Logger>();
    };

    void update(ecs::dt_t dt) override final {
        auto&& [x, y] = transform->getPos();
        logger->addMessage(
            "Pos: " + std::to_string(x) + ", " + std::to_string(y)
        );
        switch (input_) {
            case 'w':
                y -= 1;
                break;
            case 'a':
                x -= 1;
                break;
            case 's':
                y += 1;
                break;
            case 'd':
                x += 1;
                break;
        }
        input_ = '\0';
        transform->setPosXY(x, y);
    };

    void passInput(char input) {
        logger->addMessage("Input: " + std::to_string(input));
        input_ = input;
    }
};
};  // namespace ecs
