project(tests)

set(CMAKE_CXX_STANDARD 20 REQUIRED)

set(TEST_SRCS
  game/test_program.cpp
  game/test_player.cpp
  game/test_item.cpp
  game/test_room.cpp
  game/test_menu.cpp
  game/test_map.cpp
  game/test_mob.cpp
)

foreach(TestSrc ${TEST_SRCS})
  get_filename_component(TName ${TestSrc} NAME_WE)
  add_executable(${TName} ${TestSrc} $<TARGET_OBJECTS:roguelike_objects>)
  target_include_directories(${TName} PRIVATE ${DICE_CRAWL_INCLUDE})
  target_link_libraries(${TName} PRIVATE ${DICE_CRAWL_LIBS})
  add_test(NAME ${TName} COMMAND ${TName})
endforeach()

configure_file("assets/map.txt" "assets/map.txt" COPYONLY)
