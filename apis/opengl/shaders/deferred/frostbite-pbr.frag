#version 410

struct LightColors {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    float linear;
    float quadratic;

    LightColors colors;
};

const float PI = 3.14159265359;

vec3 calculatePosition(vec2 texCoord);
vec3 calculatePointLight(PointLight lightSource, vec3 normal, vec3 position, vec2 texCoord);

LightColors getLightingColors(LightColors colors, vec2 texCoord);

uniform sampler2D gDepth;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoMetallic;
uniform sampler2D gAORoughness;

uniform vec3 cameraPos;
uniform PointLight light;

in mat4 inversePerspective;

out vec4 color;

void main() {
    // TODO: move resolution to uniform
    vec2 texCoord = gl_FragCoord.xy / vec2(2560, 1440);
    vec3 position = calculatePosition(texCoord);
    vec3 normal = texture(gNormal, texCoord).xyz;

    vec3 lighting = calculatePointLight(light, normal, position, texCoord);

    color = vec4(lighting, 1.0);
}

// === Data getters ===

vec3 calculatePosition(vec2 texCoord) {
    float depth = texture(gDepth, texCoord).r;

    vec4 clipSpacePosition = vec4(vec3(texCoord, depth) * 2.0 - 1.0, 1.0);
    vec4 viewSpacePosition = inversePerspective * clipSpacePosition;

    return viewSpacePosition.xyz / viewSpacePosition.w;
}

// === PBR ===

vec3 FresnelSchlick(vec3 F0, float F90, float u);
float fr_DisneyDiffuse(float NdotV, float NdotL, float LdotH, float roughness);
float GeometrySmithGGXCorrelated(float NdotL, float NdotV, float roughness);
float DistributionGGX(float NdotH, float roughness);

vec3 calculatePointLight(PointLight lightSource, vec3 normal, vec3 position, vec2 texCoord) {
    vec4 albedoMetal = texture(gAlbedoMetallic, texCoord);
    vec2 aoRoughness = texture(gAORoughness, texCoord).rg;

    vec3 albedo = albedoMetal.rgb;
    float metallness = albedoMetal.a;
    float ao = aoRoughness.r;
    float roughness = aoRoughness.g * aoRoughness.g;

    vec3 lightVec = light.position - position;
    vec3 lightDir = normalize(lightVec);

    float distance = length(lightVec);
    float attenuation = 1.0 /  (distance * distance);
    vec3 radiance = lightSource.colors.diffuse * attenuation * 1000;

    vec3 cameraDir = normalize(cameraPos - position);
    vec3 halfway = normalize(lightDir + cameraDir);

    vec3 N = normal;
    vec3 V = cameraDir;
    vec3 L = lightDir;
    vec3 H = halfway;

    float NdotV = abs(dot(N, V)) + 0.00001;
    float LdotH = max(0.0, dot(L, H));
    float NdotH = max(0.0, dot(N, H));
    float NdotL = max(0.0, dot(N, L));

    vec3 F0 = mix(vec3(0.04), albedo, metallness);
    float F90 = 1.0;;
    float cosTheta = max(dot(halfway, cameraDir), 0.0);

    vec3 F = FresnelSchlick(F0, F90, LdotH);
    float G = GeometrySmithGGXCorrelated(NdotL, NdotV, roughness);
    float D = DistributionGGX(NdotH, roughness);

    vec3 Fr = D * F * G / PI;
    vec3 Fd = fr_DisneyDiffuse(NdotV, NdotL, LdotH, roughness) / PI * albedo;

//    return (Fr) * radiance * ao * NdotL;
    return vec3(Fd * (1-metallness) + Fr) * radiance * ao * NdotL;

//    vec3 result = F0 * (vec3(1) - albedo) + albedo * (Fd + Fr);
//
//    return result;
}

vec3 FresnelSchlick(vec3 F0, float F90, float u) {
    return F0 + (vec3(F90) - F0) * pow(1.0 - u, 5.0);
}

float fr_DisneyDiffuse(float NdotV, float NdotL, float LdotH, float roughness) {
    float energyBias = mix(0.0, 0.5, roughness);
    float eneryFactor = mix(1.0, 1.0 / 1.51, roughness);

    float fd90 = energyBias + 2.0 * LdotH * LdotH * roughness;
    vec3 f0 = vec3(1.0);

    float lightScatter = FresnelSchlick(f0, fd90, NdotL).x;
    float viewScatter = FresnelSchlick(f0, fd90, NdotV).x;

    return lightScatter * viewScatter * eneryFactor;
}

float GeometrySmithGGXCorrelated(float NdotL, float NdotV, float roughness) {
    float roughness2 = roughness * roughness;

    float GGXV = NdotL * sqrt((-NdotV * roughness2 + NdotV) * NdotV + roughness2);
    float GGXL = NdotV * sqrt((-NdotL * roughness2 + NdotL) * NdotL + roughness2);

    return 0.5 / (GGXV + GGXL);
}

float DistributionGGX(float NdotH, float roughness) {
    float roughness2 = roughness * roughness;
    float NdotH2 = NdotH * NdotH;

    float f = (roughness2 - 1) * NdotH2 + 1.0;

    return roughness2 / (f * f);
}