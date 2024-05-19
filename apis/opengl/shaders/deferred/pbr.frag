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
vec3 calculatePointLight(PointLight lightSource, vec3 _normal, vec3 position, vec2 texCoord);

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

// ===
// Physically based lighting
// ===

vec3 FresnelSchlick(float cosTheta, vec3 albedo, float metallness);
float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);

vec3 calculatePointLight(PointLight lightSource, vec3 normal, vec3 position, vec2 texCoord) {
    vec4 albedoMetal = texture(gAlbedoMetallic, texCoord);
    vec2 aoRoughness = texture(gAORoughness, texCoord).rg;

//    vec3 albedo = pow(albedoMetal.rgb, vec3(2.2));
    vec3 albedo = albedoMetal.rgb;
    float metallness = albedoMetal.a;
    float ao = aoRoughness.r;
    float roughness = aoRoughness.g;

    vec3 lightVec = light.position - position;
    vec3 lightDir = normalize(lightVec);

    float distance = length(lightVec);
    float attenuation = 1.0 /  (distance * distance);
    vec3 radiance = lightSource.colors.diffuse * attenuation * 1000;

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

    vec3 result = (kD * albedo / PI + specular) * radiance * NdotL;
    // TODO: change to IBL
    result += kS * vec3(0.5);
    result *= ao;

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
