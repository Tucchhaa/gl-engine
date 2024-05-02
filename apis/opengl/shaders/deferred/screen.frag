#version 410 core

vec3 gammaCorrection(vec3 color);

uniform sampler2D lightedColor;

in vec2 texCoords;

out vec4 outputColor;

void main() {
    vec4 color = texture(lightedColor, texCoords);

//    outputColor = vec4(gammaCorrection(color.rgb), 1.0);
    outputColor = color;
}

vec3 gammaCorrection(vec3 color) {
    float gamma = 1.0f / 2.2f;

    return pow(color, vec3(gamma));
}