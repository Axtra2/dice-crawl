#include <game/program_mode/main_menu.hpp>
#include <game/program_mode/game.hpp>
#include <game/program.hpp>
#include <game/map.hpp>

#include <cassert>
#include <utility>

MainMenu::MainMenu()
  : Menu(
    "Main Menu",
    {
        "New Game",
        "Load Game",
        "Exit"
    },
    {
        [](Program& program){
            Map map;
            map.generate();
            program.toGame(std::move(map));
            program.init();
        },
        [](Program& program){
            Map map;
            map.load("saves/map.txt");
            program.toGame(std::move(map));
            program.init();
        },
        [](Program& program){
            program.finish();
        }
    }
) { }
