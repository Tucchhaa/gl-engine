#version 410 core

layout(location = 0) in vec3 _position;

uniform mat4 transform;

out vec4 pos;

void main() {
    gl_Position = transform * vec4(_position, 1.0f);

    pos = vec4(1.0);
}
