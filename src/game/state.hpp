#pragma once

class Renderer;

class State {
public:
    virtual bool init() = 0;
    virtual State* update(char c) = 0;
    virtual bool render(Renderer& renderer) = 0;
    virtual ~State() = default;
};
