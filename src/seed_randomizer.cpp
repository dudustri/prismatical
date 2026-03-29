#include "seed_randomizer.h"
#include <cstdlib>
#include <functional>
#include <sstream>
#include <iomanip>

std::string randomSeedName() {
    static const char* adjectives[] = {
        "absolute", "amber", "azure", "blazing", "boneless", "cobalt", "crimson",
        "crispy", "crystal", "cursed", "damp", "dark", "deep", "electric",
        "emerald", "extra", "feral", "frozen", "golden", "goofy", "haunted",
        "hollow", "illegal", "indigo", "jade", "jiggly", "kroovy", "lumpy",
        "lunar", "majestic", "molten", "naked", "neon", "obsidian", "onyx",
        "opaque", "oozing", "phantom", "pointless", "prism", "quantum", "radiant",
        "rogue", "scarlet", "silver", "soggy", "solar", "still", "tiny",
        "twilight", "ultra", "unhinged", "velvet", "violet", "vivid", "wild",
        "wobbly", "xenon", "yummy", "zen", "zesty"
    };
    static const char* nouns[] = {
        "abyss", "arc", "banana", "beam", "biscuit", "blob", "bloom", "blur",
        "burrito", "cabbage", "cactus", "cascade", "cave", "cheese", "chicken",
        "coil", "corona", "current", "curve", "dawn", "donut", "drift", "dumpling",
        "dusk", "echo", "field", "flare", "flux", "fold", "form", "fungus",
        "glow", "goblin", "hamster", "haze", "helix", "horizon", "jellyfish",
        "lasagna", "lattice", "lizard", "loop", "mist", "muffin", "node",
        "nugget", "orbit", "peak", "penguin", "pickle", "platypus", "potato",
        "pulse", "raccoon", "rift", "ripple", "shore", "signal", "sock",
        "spaghetti", "spiral", "storm", "surge", "tide", "toaster", "trumpet",
        "void", "vortex", "waffle", "wave", "web", "wire", "wizard", "yak", "zone",
        "debo", "edu", "sabri"
    };
    static const int adjCount  = sizeof(adjectives) / sizeof(adjectives[0]);
    static const int nounCount = sizeof(nouns)       / sizeof(nouns[0]);

    std::ostringstream oss;
    oss << adjectives[rand() % adjCount]
        << "-" << nouns[rand() % nounCount]
        << "-" << std::hex << std::setw(4) << std::setfill('0') << (rand() & 0xFFFF);
    return oss.str();
}

unsigned int hashSeed(const std::string& s) {
    return static_cast<unsigned int>(std::hash<std::string>{}(s));
}
