#version 410 core

layout (location = 0) out vec3 gNormal;
layout (location = 1) out vec4 gAlbedoMetal;
layout (location = 2) out vec3 gAORoughness;

uniform struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
    sampler2D roughness;
    sampler2D ao;

    vec3 Kd;
    vec3 Ks;

    float shininess;
    float roughnessFactor;
    float metalnessFactor;
} material;

in struct Vertex {
    vec3 normal;
    vec3 tangent;
    vec2 texCoord;
} data;

vec3 calculateNormal();

void main() {
    vec2 texCoord = data.texCoord;

    gNormal = calculateNormal();

    gAlbedoMetal.rgb = texture(material.diffuse, texCoord).rgb * material.Kd;
    gAlbedoMetal.a = texture(material.specular, texCoord).r * material.metalnessFactor;

    float roughness = texture(material.roughness, texCoord).r * material.roughnessFactor;
    float ao = texture(material.ao, texCoord).r;
    gAORoughness = vec3(ao, roughness, 0.0);
}

vec3 calculateNormal() {
    vec3 normal = normalize(data.normal);
    vec3 tangent = normalize(data.tangent);
    vec3 bitangent = cross(normal, tangent);

    mat3 TBN = mat3(tangent, bitangent, normal);
    vec3 normalTangentSpace = texture(material.normal, data.texCoord).rgb;

    normalTangentSpace = normalize(normalTangentSpace * 2.0 - 1.0);

    vec3 result = TBN * normalTangentSpace;

    return result;
}
