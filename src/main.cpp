#include <print>
#include <string>
#include "anim.h"
#include "config.h"
#include "seed_randomizer.h"
#include "renderer/renderer.h"
#include "input/input.h"
#include "scene/scene.h"

// keys 1.. toggle these in order; labels are echoed to the terminal on toggle
constexpr struct { Anim anim; const char* label; } kAnimControls[] = {
    { Anim::PhaseDrift, "phase drift" },
    { Anim::ColorFlow,  "color flow"  },
    { Anim::FreqMorph,  "freq morph"  },
};

int main(int argc, char* argv[]) {
    unsigned int seed = (argc > 1) ? hashSeed(argv[1]) : hashSeed("prismatical");
    std::string seedStr = (argc > 1) ? argv[1] : "prismatical";
    std::println("prismatical — seed: {}", seedStr);

    Renderer renderer(config::WINDOW_WIDTH, config::WINDOW_HEIGHT);
    if (!renderer.init()) return 1;

    Input input;
    Scene scene(renderer, seed);

    std::println("keys: [space] random seed  [1] phase drift  [2] color flow  [3] freq morph  [4] cycle wave/fractal/both");

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

        for (const auto& ctl : kAnimControls) {
            if (input.animToggleRequested(ctl.anim)) {
                scene.toggleAnim(ctl.anim);
                std::println("{}: {}", ctl.label, scene.animOn(ctl.anim) ? "on" : "off");
            }
        }
        if (input.toggleModeRequested()) {
            scene.toggleMode();
            std::println("mode: {}", scene.modeName());
        }

        scene.draw();
    }

    return 0;
}
