#include "scene.h"
#include "../config.h"
#include <cmath>
#include <limits>

float Scene::seedToColorOffset(unsigned int seed) {
    return static_cast<float>(seed) / static_cast<float>(std::numeric_limits<unsigned int>::max());
}

Scene::Scene(Renderer& renderer, unsigned int initial_seed)
    : renderer_(renderer)
    , wave_(initial_seed)
    , animDelta_(0.0f)
    , colorOffset_(seedToColorOffset(initial_seed))
    , freqTime_(0.0f)
{
    wave_.generate(renderer_.width(), renderer_.height());
}

void Scene::setSeed(unsigned int seed) {
    wave_.setSeed(seed);
    animDelta_   = 0.0f;
    colorOffset_ = seedToColorOffset(seed);  // each seed starts at its own point on the color wheel
    freqTime_    = 0.0f;
    wave_.generate(renderer_.width(), renderer_.height());
}

void Scene::draw() {
    // advance active animations each frame
    if (animPhaseDrift_) animDelta_   += config::ANIM_PHASE_SPEED;
    if (animColorFlow_)  colorOffset_ += config::ANIM_COLOR_SPEED;
    if (animFreqMorph_)  freqTime_    += config::ANIM_FREQ_SPEED;

    float freqShift = animFreqMorph_
        ? std::sin(freqTime_) * config::ANIM_FREQ_RANGE
        : 0.0f;

    if (animPhaseDrift_ || animFreqMorph_) {
        wave_.generateAnimated(renderer_.width(), renderer_.height(), animDelta_, freqShift);
    }

    renderer_.clear(config::BG_R, config::BG_G, config::BG_B);
    drawCurve();
    renderer_.present();
}

void Scene::drawCurve() {
    const auto& points = wave_.points();
    for (int i = 0; i < static_cast<int>(points.size()) - 1; i++) {
        float t = static_cast<float>(i) / static_cast<float>(points.size());
        Color c = Wave::colorAt(t, colorOffset_);
        renderer_.drawLine(
            points[i].x,   points[i].y,
            points[i+1].x, points[i+1].y,
            config::DOT_RADIUS, c.r, c.g, c.b
        );
    }
}
