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


SET(DICE_CRAWL_SRCS
  ${CMAKE_SOURCE_DIR}/src/platform/renderer.cpp
  ${CMAKE_SOURCE_DIR}/src/game/main_menu.cpp
  ${CMAKE_SOURCE_DIR}/src/game/roaming.cpp
  ${CMAKE_SOURCE_DIR}/src/game/player.cpp
  ${CMAKE_SOURCE_DIR}/src/game/game.cpp
  ${CMAKE_SOURCE_DIR}/src/game/item.cpp
  ${CMAKE_SOURCE_DIR}/src/game/tile.cpp
)

set(DICE_CRAWL_INCLUDE
  ${CMAKE_SOURCE_DIR}/src/game
  ${CMAKE_SOURCE_DIR}/src/platform
)

set(DICE_CRAWL_LIBS
  ftxui::screen
  ftxui::dom
  ftxui::component
)

add_executable(roguelike 
  src/platform/main.cpp 
  ${DICE_CRAWL_SRCS}
)
target_link_libraries(roguelike PRIVATE ${DICE_CRAWL_LIBS})
target_include_directories(roguelike PRIVATE ${DICE_CRAWL_INCLUDE})
set_property(TARGET roguelike PROPERTY CXX_STANDARD 20)

configure_file("map.txt" "map.txt" COPYONLY)


add_subdirectory(tests )

