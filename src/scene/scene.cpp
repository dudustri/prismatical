#include "scene.h"
#include "../config.h"

Scene::Scene(Renderer& renderer, unsigned int initial_seed)
    : renderer_(renderer)
    , wave_(initial_seed)
{
    wave_.generate(renderer_.width(), renderer_.height());
}

void Scene::setSeed(unsigned int seed) {
    wave_.setSeed(seed);
    wave_.generate(renderer_.width(), renderer_.height());
}

void Scene::draw() {
    renderer_.clear(config::BG_R, config::BG_G, config::BG_B);
    drawCurve();
    renderer_.present();
}

void Scene::drawCurve() {
    const auto& points = wave_.points();
    for (int i = 0; i < static_cast<int>(points.size()) - 1; i++) {
        float t = static_cast<float>(i) / static_cast<float>(points.size());
        Color c = Wave::colorAt(t);
        renderer_.drawLine(
            points[i].x,   points[i].y,
            points[i+1].x, points[i+1].y,
            config::DOT_RADIUS, c.r, c.g, c.b
        );
    }
}
