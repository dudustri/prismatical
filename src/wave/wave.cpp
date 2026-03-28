#include "wave.h"
#include "../config.h"
#include <algorithm> // std::min, std::max
#include <cmath>     // sin, cos, M_PI
#include <cstdlib>  // srand, rand

Wave::Wave(unsigned int seed)
    : seed_(seed)
    , freqA_(1.0f)
    , freqB_(1.0f)
    , delta_(0.0f)
{
    deriveparamsFromSeed();
}

void Wave::setSeed(unsigned int seed) {
    seed_ = seed;
    deriveparamsFromSeed();
}

void Wave::deriveparamsFromSeed() {
    srand(seed_);
    freqA_ = static_cast<float>((rand() % config::FREQ_RANGE_A) + 1);
    freqB_ = static_cast<float>((rand() % config::FREQ_RANGE_B) + 1);
    delta_ = (static_cast<float>(rand()) / RAND_MAX) * 2.0f * M_PI;
}

void Wave::generate(int width, int height) {
    points_.clear();

    const int steps = config::CURVE_STEPS;

    float cx = width  / 2.0f;  // screen center x — sine oscillates around 0, so we shift to the middle
    float cy = height / 2.0f;  // screen center y — same reason

    float ax = cx * config::AMPLITUDE_MARGIN;
    float ay = cy * config::AMPLITUDE_MARGIN;

    for (int i = 0; i <= steps; i++) {
        // t goes from 0 to 2pi — one full traversal of the curve
        float t = (static_cast<float>(i) / steps) * 2.0f * M_PI;

        // lissajous equations
        float x = ax * std::sin(freqA_ * t + delta_);
        float y = ay * std::sin(freqB_ * t);

        // in screen coords: (0,0) is top-left, y grows downward
        points_.push_back({
            static_cast<int>(cx + x),
            static_cast<int>(cy + y)
        });
    }
}

Color Wave::colorAt(float t) {
    float h = t * 360.0f;  // hue: position on the color wheel (0=red, 120=green, 240=blue, 360=red again)
    float s = config::COLOR_SATURATION;
    float v = config::COLOR_BRIGHTNESS;

    // each color component is offset 120° apart on the wheel
    // [&] captures h, s, v from the enclosing scope — lambda can read them without passing as args
    auto hsvComponent = [&](float offset) {
        // k = hue position relative to this component's starting sector, wrapped to [0, 6)
        float k = std::fmod(offset + h / config::HUE_SECTOR_SIZE, config::HUE_SECTOR_COUNT);

        // trapezoid shape: k ramps up, (peak_width - k) ramps down, 1 clamps the top
        // min picks the lowest — together they form a rise, hold, fall curve per component
        float shape = std::max(0.0f, std::min({k, config::HUE_PEAK_WIDTH - k, 1.0f}));

        return v - v * s * shape;  // apply brightness and saturation
    };

    return {
        static_cast<uint8_t>(hsvComponent(config::HUE_OFFSET_RED)   * 255),
        static_cast<uint8_t>(hsvComponent(config::HUE_OFFSET_GREEN)  * 255),
        static_cast<uint8_t>(hsvComponent(config::HUE_OFFSET_BLUE)   * 255),
    };
}
