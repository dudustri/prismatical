#pragma once

#include "../renderer/renderer.h"
#include "../wave/wave.h"

class Scene {
public:
    Scene(Renderer& renderer, unsigned int initial_seed);

    void setSeed(unsigned int seed);
    void draw();
    void togglePhaseDrift()  { animPhaseDrift_  = !animPhaseDrift_;  }
    void toggleColorFlow()   { animColorFlow_   = !animColorFlow_;   }
    void toggleFreqMorph()   { animFreqMorph_   = !animFreqMorph_;   }
    bool phaseDriftOn() const { return animPhaseDrift_; }
    bool colorFlowOn()  const { return animColorFlow_;  }
    bool freqMorphOn()  const { return animFreqMorph_;  }

private:
    Renderer&    renderer_;
    Wave         wave_;

    float animDelta_;      // phase offset
    float colorOffset_;    // hue rotation
    float freqTime_;       // oscillates sin back and forth
    bool animPhaseDrift_  = false;
    bool animColorFlow_   = false;
    bool animFreqMorph_   = false;

    void drawCurve();
    static float seedToColorOffset(unsigned int seed);
};
