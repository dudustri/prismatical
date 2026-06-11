#include "scene.h"
#include "../config.h"
#include "../palette/palette.h"
#include <cmath>
#include <limits>

float Scene::seedToColorOffset(unsigned int seed) {
    return static_cast<float>(seed) / static_cast<float>(std::numeric_limits<unsigned int>::max());
}

const char* Scene::modeName() const {
    switch (mode_) {
        case Mode::WAVE:    return "wave";
        case Mode::FRACTAL: return "fractal";
        case Mode::BOTH:    return "both";
    }
    return "wave";
}

Scene::Scene(Renderer& renderer, unsigned int initial_seed)
    : renderer_(renderer)
    , wave_(initial_seed)
    , fractal_(initial_seed)
    , phaseTicks_(0.0f)
    , colorOffset_(seedToColorOffset(initial_seed))
    , freqTicks_(0.0f)
{
    wave_.generate(renderer_.width(), renderer_.height());
    fractal_.generate(renderer_.width(), renderer_.height());
}

void Scene::setSeed(unsigned int seed) {
    wave_.setSeed(seed);
    fractal_.setSeed(seed);                  // same seed → wave and fractal are visual cousins
    phaseTicks_  = 0.0f;
    colorOffset_ = seedToColorOffset(seed);  // each seed starts at its own point on the color wheel
    freqTicks_   = 0.0f;
    wave_.generate(renderer_.width(), renderer_.height());
    fractal_.generate(renderer_.width(), renderer_.height());
}

void Scene::draw() {
    // advance neutral counters; each pattern scales them by its own speed when drawing
    if (animOn(Anim::PhaseDrift)) phaseTicks_  += 1.0f;
    if (animOn(Anim::ColorFlow))  colorOffset_ += config::ANIM_COLOR_SPEED;
    if (animOn(Anim::FreqMorph))  freqTicks_   += 1.0f;

    renderer_.clear(config::BG_R, config::BG_G, config::BG_B);
    if (mode_ == Mode::FRACTAL || mode_ == Mode::BOTH) drawFractal();  // background layer
    if (mode_ == Mode::WAVE    || mode_ == Mode::BOTH) drawWave();     // curve on top
    renderer_.present();
}

void Scene::drawWave() {
    float animDelta = phaseTicks_ * config::ANIM_PHASE_SPEED;
    float freqShift = animOn(Anim::FreqMorph)
        ? std::sin(freqTicks_ * config::ANIM_FREQ_SPEED) * config::ANIM_FREQ_RANGE
        : 0.0f;

    // geometry only changes when the shape moves; color is applied below every frame
    if (animOn(Anim::PhaseDrift) || animOn(Anim::FreqMorph)) {
        wave_.generateAnimated(renderer_.width(), renderer_.height(), animDelta, freqShift);
    }

    const auto& points = wave_.points();
    for (int i = 0; i < static_cast<int>(points.size()) - 1; i++) {
        float t = static_cast<float>(i) / static_cast<float>(points.size());
        Color c = palette::colorAt(t, colorOffset_);
        renderer_.drawLine(
            points[i].x,   points[i].y,
            points[i+1].x, points[i+1].y,
            config::DOT_RADIUS, c.r, c.g, c.b
        );
    }
}

void Scene::drawFractal() {
    // escape field only changes when c rotates or the zoom oscillates — recompute then, not every frame
    if (animOn(Anim::PhaseDrift) || animOn(Anim::FreqMorph)) {
        float cAngle = phaseTicks_ * config::FRACTAL_C_SPEED;
        float zoom   = config::FRACTAL_ZOOM
            + (animOn(Anim::FreqMorph) ? std::sin(freqTicks_ * config::FRACTAL_ZOOM_SPEED) * config::FRACTAL_ZOOM_RANGE : 0.0f);
        fractal_.generateAnimated(renderer_.width(), renderer_.height(), cAngle, zoom);
    }

    // color the cached escape field — cheap, so color flow stays smooth without recomputing the fractal
    const auto& escape = fractal_.escape();
    int width = renderer_.width();
    int idx = 0;
    for (int y = 0; y < renderer_.height(); y++) {
        for (int x = 0; x < width; x++, idx++) {
            float t = escape[idx];
            if (t < 0.0f) continue;          // inside the set → leave the background showing
            Color c = palette::colorAt(t, colorOffset_);
            renderer_.setPixel(x, y, c.r, c.g, c.b);
        }
    }
}
