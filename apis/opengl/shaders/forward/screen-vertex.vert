#version 410 core

layout(location = 0) in vec2 _position;
layout(location = 1) in vec2 _texCoords;

out vec2 texCoords;

void main() {
    gl_Position = vec4(_position, 0, 1);
    
    texCoords = _texCoords;
}
