#pragma once

#include <ecs/gameobject.hpp>

class Scene {
   private:
    std::vector<ecs::GameObject> gameObjects_;
    bool isRunning = false;

   public:
    Scene() {}

    void addObjectToScene(ecs::GameObject o) {
        gameObjects_.emplace_back(std::move(o));
        if (isRunning) {
            o.start();
        }
    }

    void startScene() {
        if (!isRunning) {
            isRunning = true;
            for (ecs::GameObject o : gameObjects_) {
                o.start();
            }
        }
    }

    void updateScene() {
        if (isRunning) {
            for (ecs::GameObject o : gameObjects_) {
                o.update(0);  // FIXME pass actual dt
            }
        }
    }
};
