#include <iostream>
#include <cstdlib>   // rand, srand
#include <ctime>     // time — used to seed the random generator
#include "renderer/renderer.h"
#include "wave/wave.h"

int main(int argc, char* argv[]) {
    srand(static_cast<unsigned int>(time(nullptr)));

    unsigned int seed = (argc > 1) ? std::stoul(argv[1]) : 1234;
    std::cout << "prismatical — seed: " << seed << "\n";

    Renderer renderer(1600, 1200);
    if (!renderer.init()) return 1;

    Wave wave(seed);
    wave.generate(renderer.width(), renderer.height());

    while (renderer.running()) {
        renderer.pollEvents();

        // check if space was pressed — renderer exposes this via seedRequested()
        if (renderer.seedRequested()) {
            seed = static_cast<unsigned int>(rand());
            std::cout << "new seed: " << seed << "\n";
            wave.setSeed(seed);
            wave.generate(renderer.width(), renderer.height());
        }

        renderer.clear(30, 30, 33);

        const auto& points = wave.points();
        for (int i = 0; i < static_cast<int>(points.size()); i++) {
            // t = 0.0 at start of curve, 1.0 at end — drives color cycle
            float t = static_cast<float>(i) / static_cast<float>(points.size());
            Color c = Wave::colorAt(t);
            renderer.drawDot(points[i].x, points[i].y, 3, c.r, c.g, c.b);
        }

        renderer.present();
    }

    return 0;
}
