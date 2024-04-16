#version 410 core

layout(location = 0) in vec3 _position;
layout(location = 1) in vec3 _normal;
layout(location = 2) in vec2 _texCoord;
layout(location = 3) in vec3 _tangent;

uniform mat4 transform;
uniform mat3 normalTransform;
uniform mat4 perspective;
uniform mat4 lightPerspective;

out vec3 fragPos;
out vec4 fragPosLightSpace;
out vec3 normal;
out vec2 texCoord;
out vec3 tangent;

void main() {
    vec4 worldPos = transform * vec4(_position, 1.0f);
    
    gl_Position = perspective * worldPos;

    fragPos = worldPos.xyz;
    fragPosLightSpace = lightPerspective * worldPos;
    normal = normalTransform * _normal;
    texCoord = _texCoord;

    tangent = normalTransform * _tangent;
}
