#include <iostream>
#include "renderer/renderer.h"

int main(int argc, char* argv[]) {
    unsigned int seed = (argc > 1) ? std::stoul(argv[1]) : 42;
    std::cout << "prismatical — seed: " << seed << "\n";

    Renderer renderer(800, 600);

    if (!renderer.init()) {
        return 1;  // init() already printed the error
    }

    // main loop — runs until window is closed or ESC is pressed
    while (renderer.running()) {
        renderer.pollEvents();
        renderer.clear(0, 0, 0);        // black background

        // draw a test pixel in the center — white
        renderer.setPixel(400, 300, 255, 255, 255);

        renderer.present();
    }

    return 0;
}
