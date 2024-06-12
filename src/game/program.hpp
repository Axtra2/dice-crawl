#pragma once

// #include <program_mode/main_menu.hpp>
#include <game/map.hpp>

#include <memory>

class ProgramMode;
class MainMenu;
class Renderer;
class EscMenu;
class Player;
class Game;

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
    void toGameOver();
    void toLevelUp(Player& player);

    void finish();
    bool finished() const;

private:
    std::shared_ptr<ProgramMode> mainMenu_;
    std::shared_ptr<ProgramMode> escMenu_;
    std::shared_ptr<ProgramMode> game_;
    std::shared_ptr<ProgramMode> gameOver_;
    std::shared_ptr<ProgramMode> levelUp_;

    std::weak_ptr<ProgramMode> mode_;

    bool isFinished = false;
};
