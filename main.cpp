#include "apis/opengl/include.hpp"
#include "demos/backpack-demo.hpp"
#include "demos/collision-demo.hpp"
#include "demos/fps-demo.hpp"
#include "demos/many-light-demo.hpp"
#include "demos/tunnel-demo.hpp"

/*
TODO:
Optimizations:
1) do not unbind textures in different shaders
2) render objects with same material in one batch
3) use stencil pass for light volumes: https://ogldev.org/www/tutorial37/tutorial37.html
4) use uniform buffer objects

Features:
1) Bloom
2) Deferred + Forward renderings for transparency

Other:
use absolute path for includes

Plan:
1) Port to Windows
2) Create scene editor
 */

int main() {
    IEngine* engine = new GlEngine();

    // CollisionDemo scene;
    // ManyLightsDemo scene;
    // TunnelDemo scene;
    FpsDemo scene;
    // BackpackDemo scene;

    engine->setScene(&scene);

    engine->start();

    return 0;
}
