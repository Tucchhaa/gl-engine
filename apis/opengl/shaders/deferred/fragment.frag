#version 330 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

uniform struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
    float shininess;
} material;

in struct OUTPUT {
    vec3 fragPos;
    vec3 normal;
    vec3 tangent;
    vec2 texCoord;
} data;

vec3 calculateNormal();

void main() {
    gPosition = data.fragPos;
    gNormal = calculateNormal();

    gAlbedoSpec.rgb = texture(material.diffuse, data.texCoord).rgb;
    gAlbedoSpec.a = texture(material.specular, data.texCoord).r;
}

vec3 calculateNormal() {
    vec3 normal = normalize(data.normal);
    vec3 tangent = normalize(data.tangent);
    vec3 bitangent = cross(normal, tangent);

    mat3 TBN = mat3(tangent, bitangent, normal);
    vec3 normalMap = texture(material.normal, data.texCoord).rgb;

    normalMap = normalize(normalMap * 2.0 - 1.0);

    vec3 result = normalize(TBN * normalMap);

    return result;
}
