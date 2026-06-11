#pragma once

#include <vector>

class Fractal {
public:
    Fractal(unsigned int seed);

    void generate(int width, int height);
    void generateAnimated(int width, int height, float cAngleOffset, float zoom);

    // per pixel (row-major): escape fraction in [0,1), or -1 for points inside the set
    const std::vector<float>& escape() const { return escape_; }

    void setSeed(unsigned int seed);
    unsigned int seed() const { return seed_; }

private:
    unsigned int seed_;
    std::vector<float> escape_;

    float cRadius_;  // |c| — derived from seed
    float cAngle_;   // arg(c) — derived from seed, rotated by phase drift

    void deriveParamsFromSeed();
};
