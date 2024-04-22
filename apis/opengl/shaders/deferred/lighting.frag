#version 410 core

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

vec3 calculatePosition(vec2 texCoord);
vec3 calculatePointLight(LightColors lightColors, vec3 _normal, vec3 position, vec3 cameraDir);

LightColors getLightingColors(LightColors colors, vec2 texCoord);
float calculateBlinnSpecularCoefficient(vec3 cameraDir, vec3 lightDir, vec3 normal);

uniform sampler2D gDepth;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

uniform vec3 cameraPos;

uniform PointLight light;

in mat4 inversePerspective;

layout (location = 0) out vec4 color;

void main() {
    vec2 texCoord = gl_FragCoord.xy / vec2(2560, 1440);
    vec3 position = calculatePosition(texCoord);
    vec3 normal = texture(gNormal, texCoord).rgb;

    vec3 cameraDir = normalize(cameraPos - position);

    LightColors lightColors = getLightingColors(light.colors, texCoord);
    vec3 result = calculatePointLight(lightColors, normal, position, cameraDir);

    color = vec4(result, 1.0);
}

vec3 calculatePosition(vec2 texCoord) {
    float depth = texture(gDepth, texCoord).r;

    vec4 clipSpacePosition = vec4(vec3(texCoord, depth) * 2.0 - 1.0, 1.0);
    vec4 viewSpacePosition = inversePerspective * clipSpacePosition;

    return viewSpacePosition.xyz / viewSpacePosition.w;
}

vec3 calculatePointLight(LightColors colors, vec3 _normal, vec3 position, vec3 cameraDir) {
    vec3 lightVec = light.position - position;
    float _distance = length(lightVec);

    vec3 lightDir = normalize(lightVec);

    float diffuse  = max(dot(_normal, lightDir), 0.0);
    float specular = calculateBlinnSpecularCoefficient(cameraDir, lightDir, _normal);
    float attenuation = 1.0 / (1.0 + light.linear * _distance + light.quadratic * _distance * _distance);

    vec3 lit = (colors.specular * specular + colors.diffuse * diffuse);

    return  (lit + colors.ambient) * attenuation;
}

LightColors getLightingColors(LightColors lightColors, vec2 texCoord) {
    LightColors result;

    float specular = texture(gAlbedoSpec, texCoord).a;
    vec3 diffuse = texture(gAlbedoSpec, texCoord).rgb;

    result.specular = vec3(specular, specular, specular) * lightColors.specular;
    result.diffuse  = diffuse * lightColors.diffuse;
    result.ambient  = result.diffuse * lightColors.ambient;

    return result;
}

float calculateBlinnSpecularCoefficient(vec3 cameraDir, vec3 lightDir, vec3 normal) {
    vec3 halfwayDir = normalize(cameraDir + lightDir);

    // TODO: refactor
    float shininess = 32.0;

    return pow(max(dot(normal, halfwayDir), 0.0), shininess * 3);
}
