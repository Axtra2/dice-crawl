project(tests)

set(CMAKE_CXX_STANDARD 20 REQUIRED)

add_executable(test_program
  game/test_program.cpp
  $<TARGET_OBJECTS:roguelike_objects>
)
target_include_directories(test_program PRIVATE ${DICE_CRAWL_INCLUDE})
target_link_libraries(test_program PRIVATE ${DICE_CRAWL_LIBS})
add_test(NAME "program_test" COMMAND test_program)

add_executable(test_menu
  game/test_menu.cpp
  $<TARGET_OBJECTS:roguelike_objects>
)
target_include_directories(test_menu PRIVATE ${DICE_CRAWL_INCLUDE})
target_link_libraries(test_menu PRIVATE ${DICE_CRAWL_LIBS})
add_test(NAME "menu_test" COMMAND test_menu)

add_executable(test_player
  game/test_player.cpp
  $<TARGET_OBJECTS:roguelike_objects>
)
target_include_directories(test_player PRIVATE ${DICE_CRAWL_INCLUDE})
target_link_libraries(test_player PRIVATE ${DICE_CRAWL_LIBS})
add_test(NAME "player_test" COMMAND test_player)
