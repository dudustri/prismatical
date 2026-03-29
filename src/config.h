#pragma once

namespace config {

    // window
    constexpr int WINDOW_WIDTH  = 1200;
    constexpr int WINDOW_HEIGHT = 900;

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
    constexpr float COLOR_HUE_SPREAD  = 90.0f;  // how many degrees of the color wheel each oscillation swings (360 = full rainbow)
    constexpr float COLOR_HUE_CYCLES  = 5.0f;   // how many color oscillations appear along the curve — more = shorter color patches

    // animation speeds
    constexpr float ANIM_PHASE_SPEED = 0.001f;  // how fast delta drifts per frame (option 1)
    constexpr float ANIM_COLOR_SPEED = 0.005f;  // how fast hue offset rotates per frame (option 2)
    constexpr float ANIM_FREQ_SPEED  = 0.00002f;  // how fast freqA drifts per frame (option 3)
    constexpr float ANIM_FREQ_RANGE  = 8.0f;    // max ± freq drift around base value (option 3)

    // hsv color wheel constants
    constexpr float HUE_DEGREES_FULL  = 360.0f; // full color wheel in degrees
    constexpr float HUE_OFFSET_WRAP   = 1.0f;   // colorOffset wraps at 1.0 (represents one full rotation)
    constexpr float HUE_SECTOR_SIZE   = 60.0f;  // wheel divided into 6 sectors of 60° each
    constexpr float HUE_SECTOR_COUNT  = 6.0f;   // total sectors — used for fmod wrap
    constexpr float HUE_PEAK_WIDTH    = 4.0f;   // sectors each component stays at full brightness
    constexpr float HUE_OFFSET_RED    = 5.0f;   // red   component starts at sector 5 (300°)
    constexpr float HUE_OFFSET_GREEN  = 3.0f;   // green component starts at sector 3 (180°)
    constexpr float HUE_OFFSET_BLUE   = 1.0f;   // blue  component starts at sector 1 ( 60°)
    constexpr float HUE_CYCLE_RADIANS = 2.0f;   // one full sine cycle = 2 * PI radians

}
