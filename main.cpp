#include "apis/opengl/include.hpp"
#include "demos/backpack-demo.hpp"
#include "demos/collision-demo.hpp"
#include "demos/many-light-demo.hpp"
#include "demos/tunnel-demo.hpp"

/*
TODO:
Optimizations:
1) Normal mapping - use tangent space
2) do not unbind textures in different shaders
3) render objects with same material in one batch
4) instancing for meshes
5) instancing for light volumes
6) use stencil pass for light volumes: https://ogldev.org/www/tutorial37/tutorial37.html
7) use uniform buffer objects

Features:
1) Bloom
2) Deferred + Forward renderings for transparency
3) PBR
4) Physics
5) Compatibility with Windows

Other:
check for memory leaks
use absolute path for includes
 */

int main() {
    IEngine* engine = new GlEngine();

    CollisionDemo scene;
    engine->setScene(&scene);

    engine->start();

    return 0;
}
