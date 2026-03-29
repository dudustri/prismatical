#pragma once

#include <cstdint>
#include <vector>

struct Point {
    int x;
    int y;
};

struct Color {
    uint8_t r, g, b;
};

class Wave {
public:
    Wave(unsigned int seed);
    void generate(int width, int height);
    void generateAnimated(int width, int height, float animDelta, float freqShift);
    const std::vector<Point>& points() const { return points_; }
    static Color colorAt(float t, float colorOffset = 0.0f);
    void setSeed(unsigned int seed);
    unsigned int seed() const { return seed_; }

private:
    unsigned int seed_;
    std::vector<Point> points_;

    float freqA_;
    float freqB_;
    float delta_;

    void deriveparamsFromSeed();
};
