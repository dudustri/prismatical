#include "wave.h"
#include "../config.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>

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
    generateAnimated(width, height, 0.0f, 0.0f);
}

void Wave::generateAnimated(int width, int height, float animDelta, float freqShift) {
    points_.clear();

    const int steps = config::CURVE_STEPS;

    // sine oscillates around 0 so we shift to the middle
    float cx = width  / 2.0f;
    float cy = height / 2.0f;

    float ax = cx * config::AMPLITUDE_MARGIN;
    float ay = cy * config::AMPLITUDE_MARGIN;
    float effectiveDelta = delta_ + animDelta;
    float effectiveFreqA = freqA_ + freqShift;

    for (int i = 0; i <= steps; i++) {
        float t = (static_cast<float>(i) / steps) * 2.0f * M_PI;

        float x = ax * std::sin(effectiveFreqA * t + effectiveDelta);
        float y = ay * std::sin(freqB_ * t);

        points_.push_back({
            static_cast<int>(cx + x),
            static_cast<int>(cy + y)
        });
    }
}

Color Wave::colorAt(float t, float colorOffset) {
    // sine-oscillated hue along the curve — colorOffset rotates the base, CYCLES/SPREAD control patch count and width
    float base = std::fmod(colorOffset, config::HUE_OFFSET_WRAP) * config::HUE_DEGREES_FULL;
    float h = base + std::sin(t * config::HUE_CYCLE_RADIANS * M_PI * config::COLOR_HUE_CYCLES) * config::COLOR_HUE_SPREAD;  // hue: position on the color wheel (0=red, 120=green, 240=blue, 360=red again)
    float s = config::COLOR_SATURATION;
    float v = config::COLOR_BRIGHTNESS;

    // lambda function with [&] captures h, s, v from the enclosing scope
    auto hsvComponent = [&](float offset) {
        float k = std::fmod(offset + h / config::HUE_SECTOR_SIZE, config::HUE_SECTOR_COUNT);
        // clamp to a trapezoid: each color fades in, stays bright, then fades out across the wheel
        float shape = std::max(0.0f, std::min({k, config::HUE_PEAK_WIDTH - k, 1.0f}));

        return v - v * s * shape;  // apply brightness and saturation
    };

    return {
        static_cast<uint8_t>(hsvComponent(config::HUE_OFFSET_RED)   * 255),
        static_cast<uint8_t>(hsvComponent(config::HUE_OFFSET_GREEN)  * 255),
        static_cast<uint8_t>(hsvComponent(config::HUE_OFFSET_BLUE)   * 255),
    };
}
