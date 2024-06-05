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
            program.toGame(MapBuilder().build());
            program.init();
        },
        [](Program& program){
            program.toGame(MapBuilder().file("saves/map.txt").build());
            program.init();
        },
        [](Program& program){
            program.finish();
        }
    }
) { }
