#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>

#include <game.h>

#include <random>

std::mt19937 rng(std::random_device{}());

int main(void) {
    using namespace std::string_literals;
    using namespace ftxui;

    auto room = generateMapGeometry(20, 20, rng);
    // std::vector<std::string> room = {
    //     "###..#.#",
    //     "#......#",
    //     "#......#",
    //     "#..#...#",
    //     "...#....",
    //     "#......#",
    //     "#......#",
    //     "###..#.#",
    // };

    std::uniform_int_distribution<uint8_t> dis;

    // int px = room[0].size() / 2;
    // int py = room.size() / 2;
    int px = room.width() / 2;
    int py = room.height() / 2;

    int mouse_x = 0;
    int mouse_y = 0;

    auto renderer = Renderer([&] {
        // auto c = Canvas(room[0].size() * 2, room.size() * 4);
        auto c = Canvas(room.width() * 2, room.height() * 4);
        Color col = Color::Black;
        for (int y = 0; y < c.height(); ++y) {
            for (int x = 0; x < c.width(); ++x) {
                // if (x / 2 != px || y / 4 != py) {
                    // switch (room[y / 4][x / 2]) {
                    switch (room(x / 2, y / 4)) {
                    case '.':
                        c.DrawBlock(x, y, true, Color::GrayDark);
                        break;
                    case '#':
                        c.DrawBlock(x, y, true, Color::White);
                        break;
                    }
                // }
            }
        }
        // for (int i = 0; i < 4; ++i) {
        //     for (int j = 0; j < 2; ++j) {
        //         c.DrawBlock(px * 2 + j, py * 4 + i, true, Color::Red);
        //     }
        // }
        // c.DrawBlock(px, py, true, )
        // c.DrawBlock(px * 2, py * 4, "x");
        // c.DrawBlock(px * 2, py * 4, "x");
        // c.DrawBlock(px * 2, py * 4, "x");
        // c.DrawBlock(px * 2, py * 4, "x");
        // for (int y = 0, i = std::max(0, (int)events.size() - c.height() / 4); i < events.size(); ++i, y += 4) {
        //     c.DrawText(0, y, events[i]);
        // }
        // for (int i = 0; auto& s : events) {
        //     c.DrawText(0, i, s);
        //     i += 4;
        //     if (i == c.height()) {
        //         break;
        //     }
        // }
        c.DrawText(px * 2, py * 4, "x", [](Pixel& p){
            p.background_color = Color::GrayDark;
            p.foreground_color = Color::Red;
        });
        return canvas(std::move(c));
    });

    auto renderer_with_events = CatchEvent(renderer, [&](Event e){
        if (e.is_mouse()) {
            mouse_x = e.mouse().x * 2;
            mouse_y = e.mouse().y * 4;
        }
        std::string input = e.character();
        if (input.size() != 0) {
            int oldPy = py;
            int oldPx = px;
            switch (input.back()) {
            case 'w':
                py -= 1;
                break;
            case 'a':
                px -= 1;
                break;
            case 's':
                py += 1;
                break;
            case 'd':
                px += 1;
                break;
            }
            // px %= room[0].size();
            // py %= room.size();
            px %= room.width();
            py %= room.height();
            // if (room[py][px] == '#') {
            if (room(px, py) == '#') {
                px = oldPx;
                py = oldPy;
            }
        }
        return false;
    });

    auto screen = ScreenInteractive::Fullscreen();
    screen.Loop(renderer_with_events);

    return EXIT_SUCCESS;
}
