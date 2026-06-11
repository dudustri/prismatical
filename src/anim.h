#pragma once

// The real-time animation toggles, shared by Input (key binding), Scene (state),
// and main (labels). Adding one here + a label in main + keeping keys 1.. consecutive
// is all it takes to wire a new toggle.
enum class Anim { PhaseDrift, ColorFlow, FreqMorph, Count };

inline constexpr int ANIM_COUNT = static_cast<int>(Anim::Count);
