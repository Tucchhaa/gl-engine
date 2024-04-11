#version 410 core

layout(location = 0) in vec3 _position;

uniform mat4 transform;
uniform mat4 perspective;

out vec4 pos;

void main() {
//    gl_Position = perspective * transform * vec4(_position, 1.0f);

    vec4 worldPos = transform * vec4(_position, 1.0f);

    gl_Position = perspective * worldPos;

    pos = perspective * worldPos;
}
