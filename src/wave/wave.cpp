#include "wave.h"
#include <cmath>    // sin, cos, M_PI
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
    freqA_ = static_cast<float>((rand() % 423) + 1);
    freqB_ = static_cast<float>((rand() % 333) + 1);
    delta_ = (static_cast<float>(rand()) / RAND_MAX) * 2.0f * M_PI;
}

void Wave::generate(int width, int height) {
    points_.clear();

    const int steps = 100000;

    float cx = width  / 2.0f;
    float cy = height / 2.0f;

    float ax = cx * 0.9f;
    float ay = cy * 0.9f;

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

// HSV to RGB conversion — gives us the rainbow color cycle
// h = hue 0.0 to 1.0 (red → green → blue → red)
Color Wave::colorAt(float t) {
    float h = t * 360.0f;  // hue in degrees
    float s = 0.8f;        // full saturation
    float v = 1.0f;        // full brightness

    float c  = v * s;
    float x  = c * (1.0f - std::fabs(std::fmod(h / 60.0f, 2.0f) - 1.0f));
    float m  = v - c;

    float r, g, b;
    if      (h < 60)  { r = c; g = x; b = 0; }
    else if (h < 120) { r = x; g = c; b = 0; }
    else if (h < 180) { r = 0; g = c; b = x; }
    else if (h < 240) { r = 0; g = x; b = c; }
    else if (h < 300) { r = x; g = 0; b = c; }
    else              { r = c; g = 0; b = x; }

    return {
        static_cast<uint8_t>((r + m) * 255),
        static_cast<uint8_t>((g + m) * 255),
        static_cast<uint8_t>((b + m) * 255)
    };
}
