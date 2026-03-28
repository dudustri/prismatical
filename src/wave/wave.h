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

    const std::vector<Point>& points() const { return points_; }

    static Color colorAt(float t);

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
