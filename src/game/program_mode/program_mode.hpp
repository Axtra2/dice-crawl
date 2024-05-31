#pragma once

class Renderer;
class Program;

class ProgramMode {
public:
    virtual void init(Program& program) = 0;
    virtual void update(Program& program, char c) = 0;
    virtual void render(Program& program, Renderer& renderer) = 0;
    virtual ~ProgramMode() = default;
};
