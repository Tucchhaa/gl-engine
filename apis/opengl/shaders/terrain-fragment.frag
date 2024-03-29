#version 330 core

in float Height;

out vec4 color;

void main() {
    float h = (Height + 16)/64.0f;
    color = vec4(h, h, h, 1);
}