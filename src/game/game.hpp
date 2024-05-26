#pragma once

class State;
class Renderer;

class Game {
public:
    bool init();
    bool update(char c);
    bool render(Renderer& renderer);

private:
    State* state = nullptr;
};
