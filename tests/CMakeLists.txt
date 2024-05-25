project(tests)

set(CMAKE_CXX_STANDARD 20 REQUIRED)

add_executable(test1 game/TestPlayer.cpp ${DICE_CRAWL_SRCS})
target_include_directories(test1 PRIVATE ${DICE_CRAWL_INCLUDE})

target_link_libraries(test1 PRIVATE ${DICE_CRAWL_LIBS})

add_test(NAME "player_test" COMMAND test1)