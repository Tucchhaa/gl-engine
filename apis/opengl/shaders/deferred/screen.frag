#version 410 core

uniform sampler2D lightedColor;

in vec2 texCoords;

out vec4 outputColor;

void main() {
    outputColor = texture(lightedColor, texCoords);
}
