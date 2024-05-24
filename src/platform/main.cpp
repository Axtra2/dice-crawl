#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

#include <renderer.hpp>
#include <game.hpp>

int main() {
    Game game;
    game.init();
    auto renderer = ftxui::Renderer([&] {
        alignas(ftxui::Element) char data[sizeof(ftxui::Element)];
        Renderer myRenderer(data);
        if (!game.render(myRenderer)) {
            return ftxui::text("Rendering error");
        }
        ftxui::Element* res_ptr = reinterpret_cast<ftxui::Element*>(data);
        ftxui::Element res = std::move(*res_ptr);
        std::destroy_at(res_ptr);
        return res;
    });
    auto renderer_with_events = ftxui::CatchEvent(renderer, [&](ftxui::Event e){
        if (!e.is_character()) {
            return false;
        }
        std::string input = e.character();
        game.update(input.back());
        return false;
    });

    auto screen = ftxui::ScreenInteractive::Fullscreen();
    screen.Loop(renderer_with_events);

    return EXIT_SUCCESS;
}
