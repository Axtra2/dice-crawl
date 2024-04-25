// #include <ecs/components/echo.hpp>
// #include <fstream>
// #include <ftxui/dom/elements.hpp>
// #include <ftxui/screen/screen.hpp>
// #include <iostream>
// #include <ostream>
// #include <string>
//
// #include "ftxui/component/captured_mouse.hpp"
// #include "ftxui/component/component.hpp"
// #include "ftxui/component/component_base.hpp"
// #include "ftxui/component/screen_interactive.hpp"
// #include "ftxui/dom/elements.hpp"
//
// using namespace ftxui;
//
// #include <ecs/gameobject.hpp>
// #include <scene.hpp>
//
// ButtonOption Style() {
//     auto option = ButtonOption::Animated();
//     option.transform = [](const EntryState& s) {
//         auto element = text(s.label);
//         if (s.focused) {
//             element |= bold;
//         }
//         return element | center | borderEmpty | flex;
//     };
//     return option;
// }
//
// int main() {
//     int value = 50;
//
//     std::ofstream tmp("logs");
//
//     Scene scene;
//
//     ecs::GameObject test("test");
//     ecs::Echo hwecho("Hello World!", tmp);
//     test.addComponent(&hwecho);
//
//     ecs::GameObject test2("test2");
//     ecs::Echo hwecho2("Bye, World!", tmp);
//     test2.addComponent(&hwecho2);
//
//     scene.addObjectToScene(test);
//     scene.addObjectToScene(test2);
//
//     auto btn_dec_01 =
//         Button("StartScene", [&] { scene.startScene(); }, Style());
//     auto btn_inc_01 =
//         Button("UpdateScene", [&] { scene.updateScene(); }, Style());
//     auto btn_dec_10 = Button(
//         "-10",
//         [&] {
//             value -= 10;
//             tmp << value << std::endl;
//         },
//         Style()
//     );
//     auto btn_inc_10 = Button(
//         "+10",
//         [&] {
//             value += 10;
//             tmp << value << std::endl;
//         },
//         Style()
//     );
//
//     int row = 0;
//     auto buttons = Container::Vertical({
//         Container::Horizontal({btn_dec_01, btn_inc_01}, &row) | flex,
//         Container::Horizontal({btn_dec_10, btn_inc_10}, &row) | flex,
//     });
//
//     auto component = Renderer(buttons, [&] {
//         return vbox({
//                    text("value = " + std::to_string(value)),
//                    separator(),
//                    buttons->Render() | flex,
//                }) |
//                flex | border;
//     });
//
//     auto screen = ScreenInteractive::FitComponent();
//     screen.Loop(component);
//     return 0;
// }

#include <fstream>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

#include "ecs/components/collider.hpp"
#include "ecs/components/controller.hpp"
#include "ecs/components/logger.hpp"
#include "ecs/components/transform.hpp"
#include "ecs/gameobject.hpp"

int main(void) {
    using namespace std::string_literals;
    using namespace ftxui;

    std::vector<std::string> room = {
        // clang-format off
        "###..#.#",
        "#......#",
        "#......#",
        "#..#...#",
        "...#....",
        "#......#",
        "#......#",
        "###..#.#",
        // clang-format on
    };

    std::uniform_int_distribution<uint8_t> dis;

    std::ofstream tmp("logs");

    ecs::GameObject player("player");
    ecs::Logger logger(tmp);
    player.addComponent(&logger);
    ecs::Transform transform(room[0].size() / 2, room.size() / 2);
    player.addComponent(&transform);
    ecs::Controller controller;
    player.addComponent(&controller);
    ecs::Collider collider;
    player.addComponent(&collider);
    player.start();

    int mouse_x = 0;
    int mouse_y = 0;

    auto renderer = Renderer([&] {
        auto c = Canvas(room[0].size() * 2, room.size() * 4);
        Color col = Color::Black;
        auto [px, py] = transform.getPos();
        for (int y = 0; y < c.height(); ++y) {
            for (int x = 0; x < c.width(); ++x) {
                if (x / 2 != px || y / 4 != py) {
                    switch (room[y / 4][x / 2]) {
                        // case '.':
                        //     c.DrawBlock(x, y, true, Color::Black);
                        //     break;
                        case '#':
                            c.DrawBlock(x, y, true, Color::White);
                            break;
                    }
                }
            }
        }
        c.DrawText(px * 2, py * 4, "x");
        return canvas(std::move(c));
    });

    auto renderer_with_events = CatchEvent(renderer, [&](Event e) {
        if (e.is_mouse()) {
            mouse_x = e.mouse().x * 2;
            mouse_y = e.mouse().y * 4;
        }
        std::string input = e.character();
        if (input.size() != 0) {
            controller.passInput(input.back());
        }
        player.update(0);
        return false;
    });

    auto screen = ScreenInteractive::Fullscreen();
    screen.Loop(renderer_with_events);

    return EXIT_SUCCESS;
}
