#pragma once

// #include <program_mode/main_menu.hpp>
#include <game/map.hpp>

#include <memory>

class ProgramMode;
class Renderer;

class MainMenu;
class EscMenu;
class Game;

// class Map;

class Program {
public:
    Program();

    void init();
    void update(char c);
    void render(Renderer& renderer);

    void toMainMenu();
    void toEscMenu();
    void toGame();
    void toGame(Map map);

    void finish();
    bool finished() const;

private:
    std::shared_ptr<ProgramMode> mainMenu_;
    std::shared_ptr<ProgramMode> escMenu_;
    std::shared_ptr<ProgramMode> game_;

    std::weak_ptr<ProgramMode> mode_;
};
