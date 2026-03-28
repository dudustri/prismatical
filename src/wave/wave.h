#pragma once

class Wave {
public:
    Wave(unsigned int seed);
    void generate();

    // Future: accept FFT frequency data
    // void generateFromFFT(const float* frequencies, int count);

private:
    unsigned int seed_;
};
