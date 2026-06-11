#pragma once

#include "../anim.h"
#include "../renderer/renderer.h"
#include "../wave/wave.h"
#include "../fractal/fractal.h"

class Scene {
public:
    Scene(Renderer& renderer, unsigned int initial_seed);

    void setSeed(unsigned int seed);
    void draw();
    void toggleAnim(Anim a)        { anim_[static_cast<int>(a)] = !anim_[static_cast<int>(a)]; }
    bool animOn(Anim a) const      { return anim_[static_cast<int>(a)]; }
    void toggleMode()        { mode_ = static_cast<Mode>((static_cast<int>(mode_) + 1) % 3); }
    const char* modeName() const;

private:
    enum class Mode { WAVE, FRACTAL, BOTH };  // 4 cycles through these in order

    Renderer&    renderer_;
    Wave         wave_;
    Fractal      fractal_;
    Mode         mode_ = Mode::WAVE;

    // neutral tick counters — each pattern applies its own tuned speed at draw time,
    // so wave and fractal animate correctly even when combined
    float phaseTicks_;     // frames phase drift has been active
    float colorOffset_;    // hue rotation (shared by both patterns)
    float freqTicks_;      // frames freq morph has been active
    bool  anim_[ANIM_COUNT] = {};  // indexed by Anim

    void drawWave();
    void drawFractal();
    static float seedToColorOffset(unsigned int seed);
};
