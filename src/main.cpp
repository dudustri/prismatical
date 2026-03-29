#include <print>
#include <string>
#include "config.h"
#include "seed_randomizer.h"
#include "renderer/renderer.h"
#include "input/input.h"
#include "scene/scene.h"

int main(int argc, char* argv[]) {
    unsigned int seed = (argc > 1) ? hashSeed(argv[1]) : hashSeed("prismatical");
    std::string seedStr = (argc > 1) ? argv[1] : "prismatical";
    std::println("prismatical — seed: {}", seedStr);

    Renderer renderer(config::WINDOW_WIDTH, config::WINDOW_HEIGHT);
    if (!renderer.init()) return 1;

    Input input;
    Scene scene(renderer, seed);

    std::println("keys: [space] random seed  [1] phase drift  [2] color flow  [3] freq morph");

    while (!input.quitRequested()) {
        input.poll();

        if (input.seedRequested()) {
            // terminal input takes priority over space — use typed string if available
            std::string seedStr = input.pendingSeed().empty()
                ? randomSeedName()
                : input.pendingSeed();

            seed = hashSeed(seedStr);
            std::println("new seed: {}", seedStr);
            scene.setSeed(seed);
        }

        if (input.togglePhaseRequested()) {
            scene.togglePhaseDrift();
            std::println("phase drift: {}", scene.phaseDriftOn() ? "on" : "off");
        }
        if (input.toggleColorRequested()) {
            scene.toggleColorFlow();
            std::println("color flow: {}", scene.colorFlowOn() ? "on" : "off");
        }
        if (input.toggleFreqRequested()) {
            scene.toggleFreqMorph();
            std::println("freq morph: {}", scene.freqMorphOn() ? "on" : "off");
        }

        scene.draw();
    }

    return 0;
}
