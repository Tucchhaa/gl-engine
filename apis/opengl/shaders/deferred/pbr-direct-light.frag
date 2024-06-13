#version 410

struct LightColors {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct DirectLight {
    vec3 direction;

    float intensity;
    LightColors colors;
};

const float PI = 3.14159265359;

uniform sampler2D gDepth;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoMetallic;
uniform sampler2D gAORoughness;

uniform float cascadePlaneDistances[16];
uniform int cascadeCount;

uniform DirectLight light;
uniform mat4 lightPerspectives[16];
uniform sampler2DArray shadowMap;

uniform mat4 perspective;
uniform vec3 cameraPos;

in mat4 inversePerspective;

out vec4 color;

vec3 calculatePosition(vec2 texCoord);
float calculateShadow(vec3 position, vec3 normal);
vec3 calculateDirectLight(DirectLight lightSource, vec3 _normal, vec3 position, vec2 texCoord);

void main() {
    // TODO: move resolution to uniform
    vec2 texCoord = gl_FragCoord.xy / vec2(2560, 1440);
    vec3 position = calculatePosition(texCoord);
    vec3 normal = texture(gNormal, texCoord).xyz;

    vec3 lighting = calculateDirectLight(light, normal, position, texCoord);

    color = vec4(lighting, 1.0) * (1.0 - calculateShadow(position, normal));
}

// === Data getters ===

vec3 calculatePosition(vec2 texCoord) {
    float depth = texture(gDepth, texCoord).r;

    vec4 clipSpacePosition = vec4(vec3(texCoord, depth) * 2.0 - 1.0, 1.0);
    vec4 viewSpacePosition = inversePerspective * clipSpacePosition;

    return viewSpacePosition.xyz / viewSpacePosition.w;
}

// ===
// Shadow mapping
// ===

int getCascadeLayer(vec3 position);
float calculateShadowBias(vec3 normal, int layer);

float calculateShadow(vec3 position, vec3 normal) {
    int layer = getCascadeLayer(position);

    vec4 positionLightSpace = lightPerspectives[layer] * vec4(position, 1.0);
    vec3 coords = (positionLightSpace.xyz / positionLightSpace.w) * 0.5 + 0.5;

    float currentDepth = coords.z;

    if (currentDepth  > 1.0)
        return 0.0;

    float bias = calculateShadowBias(normal, layer);

    // Percentage-Closer Filtering
    float shadow = 0.0;
    vec2 texelSize = 1.0 / vec2(textureSize(shadowMap, 0));

    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(
                shadowMap, vec3(coords.xy + vec2(x, y) * texelSize, layer)
            ).r;
            shadow += (currentDepth - bias) > pcfDepth ? 1.0 : 0.0;
        }
    }

    shadow /= 9.0;

    return shadow;
}

int getCascadeLayer(vec3 position) {
    vec4 positionViewSpace = perspective * vec4(position, 1.0);
    float depthValue = abs(positionViewSpace.z);

    for (int i = 0; i < cascadeCount; ++i) {
        if (depthValue < cascadePlaneDistances[i])
            return i;
    }

    return cascadeCount-1;
}

float calculateShadowBias(vec3 normal, int layer) {
    return 0.0;

    float bias = max(0.05 * (1.0 - dot(normal, -light.direction)), 0.005);

    bias *= 1 / (cascadePlaneDistances[layer] * 0.5f);

    return bias;
}

// ===
// Physically based lighting
// ===

vec3 FresnelSchlick(float cosTheta, vec3 albedo, float metallness);
float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);

vec3 calculateDirectLight(DirectLight lightSource, vec3 normal, vec3 position, vec2 texCoord) {
    vec4 albedoMetal = texture(gAlbedoMetallic, texCoord);
    vec2 aoRoughness = texture(gAORoughness, texCoord).rg;

    vec3 albedo = albedoMetal.rgb;
    float metallness = albedoMetal.a;
    float ao = aoRoughness.r;
    float roughness = aoRoughness.g;

    vec3 lightDir = normalize(-lightSource.direction);
    vec3 radiance = lightSource.colors.diffuse * light.intensity;

    vec3 cameraDir = normalize(cameraPos - position);
    vec3 halfway = normalize(lightDir + cameraDir);
    float cosTheta = max(dot(halfway, cameraDir), 0.0);

    vec3 F = FresnelSchlick(cosTheta, albedo, metallness);
    float D = DistributionGGX(normal, halfway, roughness);
    float G = GeometrySmith(normal, cameraDir, lightDir, roughness);

    float NdotL = max(dot(normal, lightDir), 0.0);
    float NdotV = max(dot(normal, cameraDir), 0.0);

    vec3 numerator = D * G * F;
    float denominator = 4.0 * NdotV * NdotL + 0.0001;
    vec3 specular = numerator / denominator;

    vec3 kS = F;
    vec3 kD = (vec3(1.0) - kS) * (1.0 - metallness);

    vec3 result = (kD * albedo / PI + specular) * radiance * NdotL * ao;

    return result;
}

vec3 FresnelSchlick(float cosTheta, vec3 albedo, float metallness) {
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallness);

    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness) {
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

float GeometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / denom;
}
