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
out mat3 TBN;

void main() {
    vec4 worldPos = transform * vec4(_position, 1.0f);
    
    gl_Position = perspective * worldPos;

    vec3 _bitangent = cross(_normal, _tangent);

    mat3 _TBN = transpose(mat3(
        normalize(vec3(transform * vec4(_tangent, 0.0f))),
        normalize(vec3(transform * vec4(_bitangent, 0.0f))),
        normalize(vec3(transform * vec4(_normal, 0.0f)))
    ));

    fragPos = worldPos.xyz;
    fragPosLightSpace = lightPerspective * worldPos;
    normal = normalTransform * _normal;
    TBN = _TBN;
    texCoord = _texCoord;
}
