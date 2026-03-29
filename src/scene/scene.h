#pragma once

#include "../renderer/renderer.h"
#include "../wave/wave.h"

class Scene {
public:
    Scene(Renderer& renderer, unsigned int initial_seed);

    void setSeed(unsigned int seed);
    void draw();

private:
    Renderer&    renderer_;  // reference — scene uses renderer but does not own it
    Wave         wave_;

    void drawCurve();
};
