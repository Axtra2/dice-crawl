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
add_executable(roguelike src/platform/main.cpp)
target_link_libraries(roguelike
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component # Not needed for this example.
)

target_include_directories(roguelike PRIVATE "src/game")
