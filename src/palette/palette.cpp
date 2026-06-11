#include "palette.h"
#include "../config.h"
#include <algorithm>
#include <cmath>

Color palette::colorAt(float t, float colorOffset) {
    // sine-oscillated hue — colorOffset rotates the base, CYCLES/SPREAD control patch count and width
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
