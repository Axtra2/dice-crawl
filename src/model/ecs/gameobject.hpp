#pragma once

#include <cstddef>
#include <cstdint>
#include <string_view>
#include <vector>

namespace ecs {
using dt_t = std::size_t;
using coord_t = std::int32_t;

class GameObject;

class Component {
   public:
    GameObject* gameObject = nullptr;
    virtual void update(dt_t dt) = 0;
    virtual void start() = 0;
    virtual ~Component(){};
};

class GameObject {
   private:
    std::vector<Component*> components_;
    const std::string_view name_;

   public:
    explicit GameObject(std::string_view name) : name_(name){};

    template <typename T>
    T* getComponent() const noexcept {
        for (Component* component : components_) {
            if (typeid(*component) == typeid(T)) {
                return dynamic_cast<T*>(component);
            }
        }
        return nullptr;
    }

    const std::string_view name() const noexcept { return name_; }

    template <typename T>
    void removeComponent() {
        for (auto&& it = components_.begin(); it != components_.end(); it++) {
            if (typeid(*it) == typeid(T)) {
                components_.erase(it);
                return;
            }
        }
    }

    void addComponent(Component* c) {
        components_.push_back(c);
        c->gameObject = this;
    };

    void update(dt_t dt) {
        for (Component* c : components_) {
            c->update(dt);
        }
    }

    void start() {
        for (Component* c : components_) {
            c->start();
        }
    }
};

class Camera : public Component {
    void update(dt_t dt) override {}
    void start() override {}
};

}  // namespace ecs
