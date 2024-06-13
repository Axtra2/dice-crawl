#pragma once

#include <game/program_mode/program_mode.hpp>

#include <functional>
#include <optional>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>

class Menu : public ProgramMode {
public:
    Menu(
        std::string name,
        std::vector<std::string> optionNames,
        std::vector<std::function<void(Program&)>> optionActions
    );
    void init(Program& program) override;
    void processInput(Program& program, char c) override;
    void update(Program& program) override;
    void render(Program& program, Renderer& renderer) override;

protected:
    std::string name_;
    int32_t selectedOption_ = 0;
    std::vector<std::string> optionNames_;
    std::vector<std::function<void(Program&)>> optionActions_;

    using Command = std::function<void(Program&)>;
    std::optional<Command> lastCommand_ = std::nullopt;
};
