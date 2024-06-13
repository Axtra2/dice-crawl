#pragma once

#include <game/program_mode/game_command.hpp>
#include <game/program_mode/program_mode.hpp>
#include <game/character/player.hpp>
#include <game/map.hpp>

#include <memory>

class GameCommand;

class Game : public ProgramMode {
public:
    void init(Program& program) override;
    void processInput(Program& program, char c) override;
    void update(Program& program) override;
    void render(Program& program, Renderer& renderer) override;

    void setMap(Map map);
private:
    int32_t selectedInventorySlot_ = 0;
    Player player_;
    Map map_;
    std::unique_ptr<GameCommand> lastCommand_ = nullptr;
};
