#version 410 core

uniform mat4 transform;
uniform mat3 normalTransform;
uniform mat4 perspective;

layout(location = 0) in vec3 _position;
layout(location = 1) in vec3 _normal;
layout(location = 2) in vec3 _tangent;
layout(location = 3) in vec2 _texCoord;

out struct OUTPUT {
    vec3 fragPos;
    vec3 normal;
    vec3 tangent;
    vec2 texCoord;
} data;

void main() {
    vec4 worldPos = transform * vec4(_position, 1.0f);

    gl_Position = perspective * worldPos;

    data.fragPos = worldPos.xyz;

    // Don't need to multiply by transform matrix, because normal matrix has rotation and scale
    data.normal  = vec3(normalTransform * _normal);
    data.tangent = vec3(normalTransform * _tangent);

    data.texCoord = _texCoord;
}
