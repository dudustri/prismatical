#pragma once

class Renderer {
public:
    Renderer(unsigned int seed);
    void init();
    void render();
    void shutdown();

private:
    unsigned int seed_;
};
