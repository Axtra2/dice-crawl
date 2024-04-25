#pragma once

#include <ecs/gameobject.hpp>
#include <utility>

namespace ecs {
class Transform : public Component {
   private:
    coord_t x_;
    coord_t y_;

    coord_t oldX_;
    coord_t oldY_;

   public:
    Transform(coord_t x, coord_t y) : x_(x), y_(y), oldX_(x), oldY_(y){};

    std::pair<coord_t, coord_t> getPos() { return std::make_pair(x_, y_); }
    std::pair<coord_t, coord_t> prevPos() {
        return std::make_pair(oldX_, oldY_);
    }

    void setPosX(coord_t x) {
        oldX_ = x_;
        x_ = x;
    }
    void setPosY(coord_t y) {
        oldY_ = y_;
        y_ = y;
    }
    void setPosXY(coord_t x, coord_t y) {
        oldX_ = x_;
        oldY_ = y_;
        x_ = x;
        y_ = y;
    }

    void rollback() {
        std::swap(x_, oldX_);
        std::swap(y_, oldY_);
    }

    void start() override {};
    void update(dt_t) override{};
};
}  // namespace ecs
