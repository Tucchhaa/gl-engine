#version 410 core

in vec4 pos;
out vec4 color;

void main() {
    color =  vec4(pos.z, pos.z, pos.z, 1.0);
}
