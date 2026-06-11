#pragma once

#include <cstdint>

struct Color {
    uint8_t r, g, b;
};

namespace palette {

    // maps a normalized value t (curve position, fractal escape fraction, …) to a color.
    // colorOffset rotates the whole wheel — used for the color-flow animation.
    Color colorAt(float t, float colorOffset = 0.0f);

}
