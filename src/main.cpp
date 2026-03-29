#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <functional>  // std::hash
#include "config.h"
#include "renderer/renderer.h"
#include "input/input.h"
#include "scene/scene.h"

int main(int argc, char* argv[]) {
    srand(static_cast<unsigned int>(time(nullptr)));

    auto hashSeed = [](const std::string& s) {
        return static_cast<unsigned int>(std::hash<std::string>{}(s));
    };

    unsigned int seed = (argc > 1) ? hashSeed(argv[1]) : hashSeed("prismatical");
    std::string seedStr = (argc > 1) ? argv[1] : "prismatical";
    std::cout << "prismatical — seed: " << seedStr << " (" << seed << ")\n";

    Renderer renderer(config::WINDOW_WIDTH, config::WINDOW_HEIGHT);
    if (!renderer.init()) return 1;

    Input input;
    Scene scene(renderer, seed);

    while (!input.quitRequested()) {
        input.poll();

        if (input.seedRequested()) {
            // terminal input takes priority over space — use typed string if available
            std::string seedStr = input.pendingSeed().empty()
                ? std::to_string(rand())
                : input.pendingSeed();

            seed = hashSeed(seedStr);
            std::cout << "new seed: " << seedStr << " (" << seed << ")\n";
            scene.setSeed(seed);
        }

        scene.draw();
    }

    return 0;
}
