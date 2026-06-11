#include "fractal.h"
#include "../config.h"
#include <algorithm>
#include <cmath>
#include <random>

Fractal::Fractal(unsigned int seed)
    : seed_(seed)
    , cRadius_(0.0f)
    , cAngle_(0.0f)
{
    deriveParamsFromSeed();
}

void Fractal::setSeed(unsigned int seed) {
    seed_ = seed;
    deriveParamsFromSeed();
}

void Fractal::deriveParamsFromSeed() {
    std::mt19937 rng(seed_);
    // keep |c| near the Julia-set boundary — that is where the intricate shapes live
    std::uniform_real_distribution<float> radiusDist(config::FRACTAL_C_RADIUS_MIN, config::FRACTAL_C_RADIUS_MAX);
    std::uniform_real_distribution<float> angleDist(0.0f, 2.0f * M_PI);

    cRadius_ = radiusDist(rng);
    cAngle_  = angleDist(rng);
}

void Fractal::generate(int width, int height) {
    generateAnimated(width, height, 0.0f, config::FRACTAL_ZOOM);
}

void Fractal::generateAnimated(int width, int height, float cAngleOffset, float zoom) {
    escape_.assign(static_cast<size_t>(width) * height, 0.0f);

    // phase drift rotates c around the origin; its magnitude stays fixed
    float angle = cAngle_ + cAngleOffset;
    float cRe   = cRadius_ * std::cos(angle);
    float cIm   = cRadius_ * std::sin(angle);

    // map pixels into the complex plane: vertical extent is ±zoom, horizontal scaled by aspect (no stretch)
    float aspect = static_cast<float>(width) / static_cast<float>(height);
    const int maxIter = config::FRACTAL_MAX_ITER;

    int idx = 0;
    for (int py = 0; py < height; py++) {
        float zi0 = ((static_cast<float>(py) / height) - 0.5f) * 2.0f * zoom;
        for (int px = 0; px < width; px++, idx++) {
            float zr = ((static_cast<float>(px) / width) - 0.5f) * 2.0f * zoom * aspect;
            float zi = zi0;

            // escape-time: iterate z = z^2 + c until z leaves the |z|<=2 disk
            int   iter = 0;
            float zr2  = zr * zr;
            float zi2  = zi * zi;
            while (zr2 + zi2 <= 4.0f && iter < maxIter) {
                zi  = 2.0f * zr * zi + cIm;
                zr  = zr2 - zi2 + cRe;
                zr2 = zr * zr;
                zi2 = zi * zi;
                iter++;
            }

            if (iter >= maxIter) {
                escape_[idx] = -1.0f;  // never escaped — inside the set
            } else {
                // continuous escape value smooths the iteration bands into gradients
                float mag    = std::sqrt(zr2 + zi2);
                float smooth = iter + 1.0f - std::log2(std::log(mag));
                escape_[idx] = std::clamp(smooth / maxIter, 0.0f, 0.999f);
            }
        }
    }
}
