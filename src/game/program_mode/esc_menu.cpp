#include <game/program_mode/esc_menu.hpp>
#include <game/program.hpp>

#include <utility>

EscMenu::EscMenu()
  : Menu(
    "Escape Menu",
    {
        "Back to Game",
        "New Game",
        "Load Game",
        "To Main Menu",
        "Exit"
    },
    {
        [](Program& program){
            program.toGame();
        },
        [](Program& program){
            program.toGame(MapBuilder().build());
            program.init();
        },
        [](Program& program){
            program.toGame(MapBuilder().file("saves/map.txt").build());
            program.init();
        },
        [](Program& program){
            program.toMainMenu();
            program.init();
        },
        [](Program& program){
            program.finish();
        }
    }
) { }
