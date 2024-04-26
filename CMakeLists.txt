cmake_minimum_required(VERSION 3.27)
project(gl_engine)

set(CMAKE_CXX_STANDARD 17)

set(DEPENDENCIES ${CMAKE_CURRENT_SOURCE_DIR}/dependencies)

#include_directories(.)

if(APPLE)
    link_libraries("-framework OpenGL")

    link_libraries(${DEPENDENCIES}/glfw_3.3.9/lib/libglfw.3.3.dylib)
    link_libraries(${DEPENDENCIES}/assimp_5.3.1/lib/libassimp.5.3.0.dylib)

    include_directories(${DEPENDENCIES}/glfw_3.3.9/include)
    include_directories(${DEPENDENCIES}/assimp_5.3.1/include)
    include_directories(${DEPENDENCIES}/headers)
endif ()

add_executable(gl_engine
        # base
        apis/base/irenderer.hpp
        apis/base/irenderer.cpp

        apis/base/iwindow.hpp
        apis/base/iresource-manager.hpp
        apis/base/iinput.hpp
        apis/base/irender-object.hpp

        # apis/opengl
        apis/opengl/include.hpp

        apis/opengl/gl-check-error.cpp
        apis/opengl/gl-check-error.hpp

        apis/opengl/window.cpp
        apis/opengl/window.hpp

        apis/opengl/input.cpp
        apis/opengl/input.hpp

#        apis/opengl/renderer.cpp
#        apis/opengl/renderer.hpp

        apis/opengl/shader.cpp
        apis/opengl/shader.hpp

        apis/opengl/resource-manager.cpp
        apis/opengl/resource-manager.hpp

        apis/opengl/render-object.cpp
        apis/opengl/render-object.hpp

        apis/opengl/deferred-renderer.cpp
        apis/opengl/deferred-renderer.hpp

        # core
        core/include.hpp

        core/hierarchy.cpp
        core/hierarchy.hpp

        core/scene.cpp
        core/scene.hpp

        core/loader.cpp
        core/loader.hpp

        core/imesh.hpp

        # core/components
        core/components/lights/light.cpp
        core/components/lights/light.hpp

        core/components/lights/direct-light.cpp
        core/components/lights/direct-light.hpp

        core/components/lights/point-light.cpp
        core/components/lights/point-light.hpp

        core/components/lights/spot-light.cpp
        core/components/lights/spot-light.hpp

        core/components/camera.cpp
        core/components/camera.hpp

        core/components/component.cpp
        core/components/component.hpp

        core/structures/material.cpp
        core/structures/material.hpp

        core/components/mesh.cpp
        core/components/mesh.hpp

        core/components/terrain.cpp
        core/components/terrain.hpp

        core/components/cubic-patch.cpp
        core/components/cubic-patch.hpp

        core/components/transform.cpp
        core/components/transform.hpp

        core/game-object.cpp
        core/game-object.hpp

        # core/structures
        core/structures/vectors.cpp
        core/structures/vectors.hpp

        core/structures/texture.cpp
        core/structures/texture.hpp

        core/structures/texture-options.cpp
        core/structures/texture-options.hpp

        # demos
        demos/backpack-demo.cpp
        demos/backpack-demo.hpp

        demos/tunnel-demo.cpp
        demos/tunnel-demo.hpp

        demos/many-light-demo.cpp
        demos/many-light-demo.hpp

        demos/gooch-shading-demo.cpp
        demos/gooch-shading-demo.hpp

        # other
        main.cpp
        apis/base/iinput.cpp

        apis/opengl/renderer.cpp
        apis/opengl/renderer.hpp
)
