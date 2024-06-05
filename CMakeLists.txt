cmake_minimum_required (VERSION 3.11)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

include(FetchContent)
set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG cdf28903a7781f97ba94d30b79c3a4b0c97ccce7
)
FetchContent_MakeAvailable(ftxui)

project(roguelike LANGUAGES CXX VERSION 1.0.0)
include(CTest)
enable_testing()

set(DICE_CRAWL_SRCS
  ${CMAKE_SOURCE_DIR}/src/platform/renderer.cpp
  ${CMAKE_SOURCE_DIR}/src/platform/files.cpp

  ${CMAKE_SOURCE_DIR}/src/game/program_mode/game_over.cpp
  ${CMAKE_SOURCE_DIR}/src/game/program_mode/main_menu.cpp
  ${CMAKE_SOURCE_DIR}/src/game/program_mode/esc_menu.cpp
  ${CMAKE_SOURCE_DIR}/src/game/program_mode/level_up.cpp
  ${CMAKE_SOURCE_DIR}/src/game/program_mode/game.cpp
  ${CMAKE_SOURCE_DIR}/src/game/program_mode/menu.cpp
  ${CMAKE_SOURCE_DIR}/src/game/character/mob/hostile.cpp
  ${CMAKE_SOURCE_DIR}/src/game/character/mob/passive.cpp
  ${CMAKE_SOURCE_DIR}/src/game/character/mob/coward.cpp
  ${CMAKE_SOURCE_DIR}/src/game/character/mob/mob.cpp
  ${CMAKE_SOURCE_DIR}/src/game/character/character.cpp
  ${CMAKE_SOURCE_DIR}/src/game/character/player.cpp
  ${CMAKE_SOURCE_DIR}/src/game/direction.cpp
  ${CMAKE_SOURCE_DIR}/src/game/program.cpp
  ${CMAKE_SOURCE_DIR}/src/game/item.cpp
  ${CMAKE_SOURCE_DIR}/src/game/tile.cpp
  ${CMAKE_SOURCE_DIR}/src/game/room.cpp
  ${CMAKE_SOURCE_DIR}/src/game/map.cpp

  ${CMAKE_SOURCE_DIR}/src/utils/random.cpp
)

set(DICE_CRAWL_WARNING_FLAGS
  -Wall
  -Wextra
  -Wshadow
  -Wpedantic
  -Wno-unused-parameter
)

set(DICE_CRAWL_INCLUDE ${CMAKE_SOURCE_DIR}/src)

set(DICE_CRAWL_LIBS
  ftxui::component
  ftxui::screen
  ftxui::dom
)

add_library(roguelike_objects OBJECT ${DICE_CRAWL_SRCS})
target_include_directories(roguelike_objects PRIVATE ${DICE_CRAWL_INCLUDE})
target_link_libraries(roguelike_objects PRIVATE ${DICE_CRAWL_LIBS})
set_property(TARGET roguelike_objects PROPERTY CXX_STANDARD 20)
target_compile_options(roguelike_objects PRIVATE ${DICE_CRAWL_WARNING_FLAGS})

add_executable(roguelike
  src/platform/main.cpp
  $<TARGET_OBJECTS:roguelike_objects>
)
target_link_libraries(roguelike PRIVATE ${DICE_CRAWL_LIBS})
target_include_directories(roguelike PRIVATE ${DICE_CRAWL_INCLUDE})
set_property(TARGET roguelike PROPERTY CXX_STANDARD 20)
target_compile_options(roguelike PRIVATE ${DICE_CRAWL_WARNING_FLAGS})

configure_file("saves/map.txt" "saves/map.txt" COPYONLY)


add_subdirectory(tests)
