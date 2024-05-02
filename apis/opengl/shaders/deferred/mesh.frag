#version 410 core

layout (location = 0) out vec3 gNormal;
layout (location = 1) out vec4 gAlbedoSpec;
layout (location = 2) out vec3 gAORoughness;

uniform struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
    sampler2D roughness;
    sampler2D ao;
    float shininess;
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

    gAlbedoSpec.rgb = texture(material.diffuse, texCoord).rgb;
    gAlbedoSpec.a = texture(material.specular, texCoord).r;

    float roughness = texture(material.roughness, texCoord).r;
    float ao = texture(material.ao, texCoord).r;
    gAORoughness = vec3(ao, roughness, 0.0);
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
