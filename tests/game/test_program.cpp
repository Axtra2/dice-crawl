#include <game/character/player.hpp>
#include <game/program.hpp>
#include <game/map.hpp>

#include <cassert>
#include <utility>

void testImmediateFinish() {
    Program program;
    assert(!program.finished());
    program.finish();
    assert(program.finished());
}

void testFinishThenMode() {
    auto test = []<class... Args>(
        void(Program::*method)(Args...),
        auto&&... args
    ){
        Program program;
        assert(!program.finished());

        program.finish();
        assert(program.finished());

        (program.*method)(std::forward<decltype(args)>(args)...);
        assert(!program.finished());
    };

    test(&Program::toMainMenu);
    test(&Program::toEscMenu);
    test.template operator()<Map>(&Program::toGame, Map());
    test(&Program::toGame);
    test(&Program::toGameOver);
    {
        Player p;
        test(&Program::toLevelUp, p);
    }
}

void testModeThenFinish() {
    auto test = []<class... Args>(
        void(Program::*method)(Args...),
        auto&&... args
    ){
        Program program;
        assert(!program.finished());

        (program.*method)(std::forward<decltype(args)>(args)...);
        assert(!program.finished());

        program.finish();
        assert(program.finished());
    };

    test(Program::toMainMenu);
    test(Program::toEscMenu);
    test.template operator()<Map>(Program::toGame, Map());
    test(Program::toGame);
    test(Program::toGameOver);

    Player p;
    test(Program::toLevelUp, p);
}

void testManyModeSwitches() {
    Program program;
    assert(!program.finished());

    program.toMainMenu();
    assert(!program.finished());

    program.toEscMenu();
    assert(!program.finished());

    program.toGame();
    assert(!program.finished());

    program.toGame(Map());
    assert(!program.finished());

    program.toGameOver();
    assert(!program.finished());

    Player p;
    program.toLevelUp(p);
    assert(!program.finished());

    program.finish();
    assert(program.finished());
}

int main() {
    testImmediateFinish();
    testFinishThenMode();
    testModeThenFinish();
    testManyModeSwitches();
}
