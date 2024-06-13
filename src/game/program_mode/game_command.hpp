#pragma once

#include <game/direction.hpp>

#include <cstdint>
#include <memory>

class Program;
class Player;
class Map;

class GameCommand {
public:
    virtual bool operator()() = 0;
    virtual ~GameCommand() = default;
};

class MoveCommand : public GameCommand {
public:
    MoveCommand(Map& map, Player& player, Direction dir);
    bool operator()() override;

private:
    Map& map_;
    Player& player_;
    Direction dir_;
};

class UseItemCommand : public GameCommand {
public:
    UseItemCommand(
        Map& map,
        Player& player,
        int32_t selectedInventorySlot
    );
    bool operator()() override;

private:
    Map& map_;
    Player& player_;
    int32_t selectedInventorySlot_;
};

class PickUpCommand : public GameCommand {
public:
    PickUpCommand(Map& map, Player& player);
    bool operator()() override;

private:
    Map& map_;
    Player& player_;
};

class EscCommand : public GameCommand {
public:
    EscCommand(Program& program);
    bool operator()() override;

private:
    Program& program_;
};

class MoveInventorySelectionCommand : public GameCommand {
public:
    MoveInventorySelectionCommand(int32_t& selectedInventorySlot, int32_t delta);
    bool operator()() override;

private:
    int32_t& selectedInventorySlot_;
    int32_t delta_;
};

class UpdateOnSuccessCommand : public GameCommand {
public:
    UpdateOnSuccessCommand(
        Map& map,
        Player& player,
        Program& program,
        std::unique_ptr<GameCommand> before
    );
    bool operator()() override;

private:
    Map& map_;
    Player& player_;
    Program& program_;
    std::unique_ptr<GameCommand> before_;
};
