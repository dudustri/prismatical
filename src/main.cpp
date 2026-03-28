#include <iostream>
#include <cstdlib>
#include <ctime>
#include "config.h"
#include "renderer/renderer.h"
#include "wave/wave.h"

int main(int argc, char* argv[]) {
    srand(static_cast<unsigned int>(time(nullptr)));

    unsigned int seed = (argc > 1) ? std::stoul(argv[1]) : 1234;
    std::cout << "prismatical — seed: " << seed << "\n";

    Renderer renderer(config::WINDOW_WIDTH, config::WINDOW_HEIGHT);
    if (!renderer.init()) return 1;

    Wave wave(seed);
    wave.generate(renderer.width(), renderer.height());

    while (renderer.running()) {
        renderer.pollEvents();

        if (renderer.seedRequested()) {
            seed = static_cast<unsigned int>(rand());
            std::cout << "new seed: " << seed << "\n";
            wave.setSeed(seed);
            wave.generate(renderer.width(), renderer.height());
        }

        renderer.clear(config::BG_R, config::BG_G, config::BG_B);

        const auto& points = wave.points();
        for (int i = 0; i < static_cast<int>(points.size()); i++) {
            float t = static_cast<float>(i) / static_cast<float>(points.size());
            Color c = Wave::colorAt(t);
            renderer.drawDot(points[i].x, points[i].y, config::DOT_RADIUS, c.r, c.g, c.b);
        }

        renderer.present();
    }

    return 0;
}
