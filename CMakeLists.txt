cmake_minimum_required(VERSION 3.27)
project(gl_engine)

set(CMAKE_CXX_STANDARD 17)

link_libraries("-framework OpenGL")

link_libraries(/opt/homebrew/Cellar/glfw/3.3.9/lib/libglfw.3.3.dylib)
link_libraries(/opt/homebrew/Cellar/assimp/5.3.1/lib/libassimp.5.3.0.dylib)

include_directories(/opt/homebrew/Cellar/glfw/3.3.9/include)
include_directories(/opt/homebrew/Cellar/assimp/5.3.1/include)
include_directories(/Users/tucha/C-libs)

add_executable(gl_engine
        apis/opengl/gl-check-error.cpp
        apis/opengl/input.cpp
        apis/opengl/renderer.cpp
        apis/opengl/shader.cpp
        core/game-object/components/lights/direct-light.cpp
        core/game-object/components/lights/light.cpp
        core/game-object/components/lights/point-light.cpp
        core/game-object/components/camera.cpp
        core/game-object/components/component.cpp
        core/game-object/components/material.cpp
        core/game-object/components/mesh.cpp
        core/game-object/components/transform.cpp
        core/game-object/game-object.cpp
        core/hierarchy.cpp
        core/scene.cpp
        main.cpp
        core/loader.cpp
        core/structures/vectors.cpp
        apis/opengl/resource-manager.cpp
)
