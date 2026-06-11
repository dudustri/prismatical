#pragma once

#include <vector>

struct Point {
    int x;
    int y;
};

class Wave {
public:
    Wave(unsigned int seed);
    void generate(int width, int height);
    void generateAnimated(int width, int height, float animDelta, float freqShift);
    const std::vector<Point>& points() const { return points_; }
    void setSeed(unsigned int seed);
    unsigned int seed() const { return seed_; }

private:
    unsigned int seed_;
    std::vector<Point> points_;

    float freqA_;
    float freqB_;
    float delta_;

    void deriveParamsFromSeed();
};
