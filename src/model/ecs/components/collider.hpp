#pragma once

#include <ecs/gameobject.hpp>
#include <tuple>

#include "ecs/components/transform.hpp"

namespace ecs {
class Collider : public Component {
   private:
    coord_t posX_;
    coord_t posY_;
    Transform* transform_;
    bool enabled_;
    bool collided_;

   public:
    Collider() : enabled_(true){};

    void disable() { enabled_ = false; }
    void enable() { enabled_ = true; }
    void setEnabled(bool enabled) { enabled_ = enabled; }

    void update(dt_t dt) override final {
        std::tie(posX_, posY_) = transform_->getPos();
    };

    void start() override final { gameObject->getComponent<Transform>(); };
};
}  // namespace ecs
