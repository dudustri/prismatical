#pragma once

namespace config {

    // window
    constexpr int WINDOW_WIDTH  = 1600;
    constexpr int WINDOW_HEIGHT = 1200;

    // background color (RGB)
    constexpr unsigned char BG_R = 30;
    constexpr unsigned char BG_G = 30;
    constexpr unsigned char BG_B = 33;

    // renderer
    constexpr int DOT_RADIUS = 2;

    // wave / lissajous
    constexpr int   CURVE_STEPS      = 200000; // more steps = denser curve
    constexpr int   FREQ_RANGE_A     = 423;    // max frequency on X axis
    constexpr int   FREQ_RANGE_B     = 333;    // max frequency on Y axis — prime, avoids common factors with A
    constexpr float AMPLITUDE_MARGIN = 0.9f;   // how close to edge the curve reaches (0-1)
    constexpr float COLOR_SATURATION = 0.85f;  // 0 = grey, 1 = full color
    constexpr float COLOR_BRIGHTNESS = 1.0f;   // 0 = black, 1 = full brightness

    // hsv color wheel constants
    constexpr float HUE_SECTOR_SIZE  = 60.0f;  // wheel divided into 6 sectors of 60° each
    constexpr float HUE_SECTOR_COUNT = 6.0f;   // total sectors — used for fmod wrap
    constexpr float HUE_PEAK_WIDTH   = 4.0f;   // sectors each component stays at full brightness
    constexpr float HUE_OFFSET_RED   = 5.0f;   // red   component starts at sector 5 (300°)
    constexpr float HUE_OFFSET_GREEN = 3.0f;   // green component starts at sector 3 (180°)
    constexpr float HUE_OFFSET_BLUE  = 1.0f;   // blue  component starts at sector 1 ( 60°)

}
