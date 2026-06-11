#include "wave.h"
#include "../config.h"
#include <cmath>
#include <random>

Wave::Wave(unsigned int seed)
    : seed_(seed)
    , freqA_(1.0f)
    , freqB_(1.0f)
    , delta_(0.0f)
{
    deriveParamsFromSeed();
}

void Wave::setSeed(unsigned int seed) {
    seed_ = seed;
    deriveParamsFromSeed();
}

void Wave::deriveParamsFromSeed() {
    std::mt19937 rng(seed_);
    std::uniform_int_distribution<int>  freqDistA(1, config::FREQ_RANGE_A);
    std::uniform_int_distribution<int>  freqDistB(1, config::FREQ_RANGE_B);
    std::uniform_real_distribution<float> deltaDistribution(0.0f, 2.0f * M_PI);

    freqA_ = static_cast<float>(freqDistA(rng));
    freqB_ = static_cast<float>(freqDistB(rng));
    delta_ = deltaDistribution(rng);
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
