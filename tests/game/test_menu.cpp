#include <game/program_mode/menu.hpp>
#include <game/program.hpp>

#include <cassert>
#include <utility>

void testMenu() {
    static constexpr char SELECT_KEY = 'e';
    static constexpr char DOWN_KEY = 'j';
    static constexpr char UP_KEY = 'k';

    const std::vector<std::string> names = {
        "", "", ""
    };

    int i1 = 0;
    int i2 = 0;
    int i3 = 0;
    const std::vector<std::function<void(Program&)>> actions {
        [&i1](Program&){++i1;},
        [&i2](Program&){++i2;},
        [&i3](Program&){++i3;}
    };

    Program program;

    Menu menu("", std::move(names), std::move(actions));
    assert(i1 == 0);
    assert(i2 == 0);
    assert(i3 == 0);

    menu.update(program, SELECT_KEY);
    assert(i1 == 1);
    assert(i2 == 0);
    assert(i3 == 0);

    menu.update(program, DOWN_KEY);
    assert(i1 == 1);
    assert(i2 == 0);
    assert(i3 == 0);

    menu.update(program, SELECT_KEY);
    assert(i1 == 1);
    assert(i2 == 1);
    assert(i3 == 0);

    menu.update(program, UP_KEY);
    assert(i1 == 1);
    assert(i2 == 1);
    assert(i3 == 0);

    menu.update(program, SELECT_KEY);
    assert(i1 == 2);
    assert(i2 == 1);
    assert(i3 == 0);

    menu.update(program, UP_KEY);
    menu.update(program, UP_KEY);
    menu.update(program, UP_KEY);
    menu.update(program, UP_KEY);
    menu.update(program, SELECT_KEY);
    assert(i1 == 3);
    assert(i2 == 1);
    assert(i3 == 0);

    menu.update(program, DOWN_KEY);
    menu.update(program, DOWN_KEY);
    menu.update(program, DOWN_KEY);
    menu.update(program, DOWN_KEY);
    menu.update(program, DOWN_KEY);
    menu.update(program, DOWN_KEY);
    menu.update(program, SELECT_KEY);
    assert(i1 == 3);
    assert(i2 == 1);
    assert(i3 == 1);
}

int main() {
    testMenu();
}
