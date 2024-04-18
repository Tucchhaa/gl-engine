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

void main() {
    gPosition = data.fragPos;
    gNormal = normalize(data.normal);
    gAlbedoSpec.rgb = texture(material.diffuse, data.texCoord).rgb;
    gAlbedoSpec.a = texture(material.specular, data.texCoord).r;
}
