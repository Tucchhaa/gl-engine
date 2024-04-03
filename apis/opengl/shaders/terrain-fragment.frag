#version 330 core

in float height;

out vec4 color;

void main() {
    float h = (height + 16)/64.0f;
    color = vec4(h, h, h, 1);
}
