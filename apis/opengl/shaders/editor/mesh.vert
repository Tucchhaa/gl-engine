#version 410

uniform mat4 transform;
uniform mat4 perspective;

layout(location = 0) in vec3 _position;

void main() {
    gl_Position = perspective * transform * vec4(_position, 1.0f);
}
