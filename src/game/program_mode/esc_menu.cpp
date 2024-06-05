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
            program.toMainMenu();
            program.init();
        },
        [](Program& program){
            program.finish();
        }
    }
) { }
