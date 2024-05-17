cmake_minimum_required(VERSION 3.27)
include(sourceFiles.cmake)

project(gl_engine)

set(CMAKE_CXX_STANDARD 17)

#include_directories(.)

find_package(glm CONFIG REQUIRED)
link_libraries(glm::glm)

find_package(glfw3 CONFIG REQUIRED)
link_libraries(glfw)

find_package(assimp CONFIG REQUIRED)
link_libraries(assimp::assimp)

if(APPLE)
    link_libraries("-framework OpenGL")

    # NEXT LINE IS FOR DEVELOPMENT ONLY
    include_directories(/Users/tucha/.vcpkg-clion/vcpkg/installed/arm64-osx/include)
endif ()

get_demos_source_files()
get_source_files()

add_executable(gl_engine
        main.cpp
        ${DEMOS_SOURCE_FILES}
        ${SOURCE_FILES}
)
