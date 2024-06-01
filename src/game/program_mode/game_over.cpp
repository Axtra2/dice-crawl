#include <game/program_mode/game_over.hpp>
#include <game/program.hpp>

GameOver::GameOver()
  : Menu(
    "Game Over",
    {
        "To Main Menu",
        "Exit"
    },
    {
        [](Program& program){
            program.toMainMenu();
            program.init();
        },
        [](Program& program){
            program.finish();
        }
    }
) { }
