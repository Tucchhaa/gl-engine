#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

//uniform mat4 perspective;
//uniform vec3 cameraPos;

out vec2 _texCoord;

void main() {
    gl_Position = vec4(position, 1);
    _texCoord = texCoord;
}