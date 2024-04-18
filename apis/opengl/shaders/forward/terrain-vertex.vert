#version 410 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

out vec2 _texCoord;

void main() {
    gl_Position = vec4(position, 1);
    _texCoord = texCoord;
}