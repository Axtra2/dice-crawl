#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

#include <platform/renderer.hpp>
#include <game/program.hpp>

int main() {
    Program program;
    program.init();

    auto screen = ftxui::ScreenInteractive::Fullscreen();
    screen.TrackMouse(false);

    auto renderer = ftxui::Renderer([&] {
        alignas(ftxui::Element) char data[sizeof(ftxui::Element)];
        Renderer myRenderer(data);

        program.render(myRenderer);

        ftxui::Element* res_ptr = std::launder(reinterpret_cast<ftxui::Element*>(data));
        ftxui::Element res = std::move(*res_ptr);
        std::destroy_at(res_ptr);
        return res;
    });

    auto renderer_with_events = ftxui::CatchEvent(renderer, [&](ftxui::Event e){
        if (!e.is_character()) {
            return false;
        }
        std::string input = e.character();

        program.update(input.back());
        if (program.finished()) {
            screen.ExitLoopClosure()();
            return true;
        }

        return false;
    });

    screen.Loop(renderer_with_events);

    return EXIT_SUCCESS;
}
