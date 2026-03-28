#pragma once

class Fractal {
public:
    Fractal(unsigned int seed);
    void generate();

private:
    unsigned int seed_;
};
