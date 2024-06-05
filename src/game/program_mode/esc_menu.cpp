#include <game/character/mob/sci_fi_mob.hpp>
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
            SciFiMobFactory mobFactory;
            program.toGame(MapBuilder().mobFactory(mobFactory).build());
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
