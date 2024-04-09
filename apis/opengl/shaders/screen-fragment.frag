#version 410 core

uniform sampler2D screenTexture;

in vec2 texCoords;

out vec4 outputColor;

vec4 blackAndWhite1(vec4 color);
vec4 blackAndWhite2(vec4 color);
vec4 negative(vec4 color);
vec4 kernelEffect(float[9] matrix);

void main() {
    float edgeDetetctionKernel[9] = float[](
        1, 1, 1,
        1, -8, 1,
        1, 1, 1
    );
    
    vec4 color = texture(screenTexture, texCoords);

    outputColor =  color;
}

vec4 blackAndWhite1(vec4 color) {
    float average = (color.r + color.b + color.g) / 3;
    return vec4(average, average, average, 1);
}

vec4 blackAndWhite2(vec4 color) {
    float average = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
    return vec4(average, average, average, 1);
}

vec4 negative(vec4 color) {
    return vec4(vec3(1 - color), 1);
}

vec4 kernelEffect(float[9] matrix) {
    const float offset = 1.0 / 300.0;

    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right
    );
    
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
        sampleTex[i] = vec3(texture(screenTexture, texCoords.st + offsets[i]));
    
    vec3 color = vec3(0.0);
    for(int i = 0; i < 9; i++)
        color += sampleTex[i] * matrix[i];
    
    return vec4(color, 1);
}
