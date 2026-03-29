#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include "config.h"
#include "seed_randomizer.h"
#include "renderer/renderer.h"
#include "input/input.h"
#include "scene/scene.h"

int main(int argc, char* argv[]) {
    srand(static_cast<unsigned int>(time(nullptr)));

    unsigned int seed = (argc > 1) ? hashSeed(argv[1]) : hashSeed("prismatical");
    std::string seedStr = (argc > 1) ? argv[1] : "prismatical";
    std::cout << "prismatical — seed: " << seedStr << "\n";

    Renderer renderer(config::WINDOW_WIDTH, config::WINDOW_HEIGHT);
    if (!renderer.init()) return 1;

    Input input;
    Scene scene(renderer, seed);

    std::cout << "keys: [space] random seed  [1] phase drift  [2] color flow  [3] freq morph\n";

    while (!input.quitRequested()) {
        input.poll();

        if (input.seedRequested()) {
            // terminal input takes priority over space — use typed string if available
            std::string seedStr = input.pendingSeed().empty()
                ? randomSeedName()
                : input.pendingSeed();

            seed = hashSeed(seedStr);
            std::cout << "new seed: " << seedStr << "\n";
            scene.setSeed(seed);
        }

        if (input.togglePhaseRequested()) {
            scene.togglePhaseDrift();
            std::cout << "phase drift: " << (scene.phaseDriftOn() ? "on" : "off") << "\n";
        }
        if (input.toggleColorRequested()) {
            scene.toggleColorFlow();
            std::cout << "color flow: " << (scene.colorFlowOn() ? "on" : "off") << "\n";
        }
        if (input.toggleFreqRequested()) {
            scene.toggleFreqMorph();
            std::cout << "freq morph: " << (scene.freqMorphOn() ? "on" : "off") << "\n";
        }

        scene.draw();
    }

    return 0;
}
