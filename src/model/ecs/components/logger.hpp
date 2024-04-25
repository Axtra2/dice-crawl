#pragma once

#include <ecs/gameobject.hpp>
#include <fstream>
#include <iostream>
#include <list>
#include <string_view>

namespace ecs {
class Logger : public ecs::Component {
    std::list<std::string_view> msgs_;
    std::ofstream& of_;

    void update(ecs::dt_t dt) override {
        if (msgs_.size() != 0) {
            for (std::string_view& str : msgs_) {
                of_ << str << std::endl;
            }
            msgs_.clear();
        }
    }
    void start() override {
        of_ << "Initializing logger in GameObject: " << gameObject->name()
            << std::endl;
    }

   public:
    Logger(std::ofstream& of) : of_(of) {}

    void addMessage(std::string_view msg) { msgs_.emplace_back(msg); }
};
}  // namespace ecs
