#include <iostream>
#include "renderer/renderer.h"
#include "fractal/fractal.h"
#include "wave/wave.h"

int main(int argc, char* argv[]) {
    unsigned int seed = (argc > 1) ? std::stoul(argv[1]) : 42;

    std::cout << "prismatical — seed: " << seed << std::endl;

    // TODO: initialize renderer
    // TODO: generate shape from seed
    // TODO: render loop

    return 0;
}
