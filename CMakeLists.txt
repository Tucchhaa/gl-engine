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
        apis/base/iwindow.hpp
        apis/base/irenderer.hpp
        apis/base/iresource-manager.hpp
        apis/base/iinput.hpp

        # apis/opengl
        apis/opengl/include.hpp

        apis/opengl/gl-check-error.cpp
        apis/opengl/gl-check-error.hpp

        apis/opengl/window.cpp
        apis/opengl/window.hpp

        apis/opengl/input.cpp
        apis/opengl/input.hpp

        apis/opengl/renderer.cpp
        apis/opengl/renderer.hpp

        apis/opengl/shader.cpp
        apis/opengl/shader.hpp

        apis/opengl/resource-manager.cpp
        apis/opengl/resource-manager.hpp

        apis/opengl/render-object.cpp
        apis/opengl/render-object.hpp

        # core
        core/include.hpp

        core/hierarchy.cpp
        core/hierarchy.hpp

        core/scene.cpp
        core/scene.hpp

        core/loader.cpp
        core/loader.hpp

        # core/game-object
        core/game-object/components/lights/light.cpp
        core/game-object/components/lights/light.hpp

        core/game-object/components/lights/direct-light.cpp
        core/game-object/components/lights/direct-light.hpp

        core/game-object/components/lights/point-light.cpp
        core/game-object/components/lights/point-light.hpp

        core/game-object/components/lights/spot-light.cpp
        core/game-object/components/lights/spot-light.hpp

        core/game-object/components/camera.cpp
        core/game-object/components/camera.hpp

        core/game-object/components/component.cpp
        core/game-object/components/component.hpp

        core/structures/material.cpp
        core/structures/material.hpp

        core/game-object/components/mesh.cpp
        core/game-object/components/mesh.hpp

        core/game-object/components/terrain.cpp
        core/game-object/components/terrain.hpp

        core/game-object/components/cubic-patch.cpp
        core/game-object/components/cubic-patch.hpp

        core/game-object/components/transform.cpp
        core/game-object/components/transform.hpp

        core/game-object/game-object.cpp
        core/game-object/game-object.hpp

        # core/structures
        core/structures/vectors.cpp
        core/structures/vectors.hpp

        core/structures/texture.cpp
        core/structures/texture.hpp

        core/structures/texture-options.cpp
        core/structures/texture-options.hpp

        # other
        main.cpp
        core/imesh.hpp
)
