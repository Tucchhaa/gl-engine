#version 410 core

#define DIRECT_LIGHTS_LENGTH 5
#define POINT_LIGHTS_LENGTH 105
#define SPOT_LIGHTS_LENGTH 5

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

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

uniform vec3 cameraPos;
uniform PointLight pointLights[POINT_LIGHTS_LENGTH];

in vec2 texCoords;

out vec4 outputColor;

vec3 calculatePointLight(PointLight light, vec3 _normal, vec3 position, vec3 cameraDir);

LightColors getLightingColors(LightColors lightColors);
float calculateBlinnSpecularCoefficient(vec3 cameraDir, vec3 lightDir, vec3 normal);

void main() {
    vec3 fragPos = texture(gPosition, texCoords).rgb;
    vec3 normal = texture(gNormal, texCoords).rgb;

    vec3 cameraDir = normalize(cameraPos - fragPos);

    vec3 result = vec3(0, 0, 0);

    for(int i=0; i < POINT_LIGHTS_LENGTH; i++) {
        result += calculatePointLight(pointLights[i], normal, fragPos, cameraDir);
    }

    outputColor = vec4(result, 1);
}

vec3 calculatePointLight(PointLight light, vec3 _normal, vec3 position, vec3 cameraDir) {
    LightColors colors = getLightingColors(light.colors);

    vec3 lightVec = light.position - position;
    float _distance = length(lightVec);

    vec3 lightDir = normalize(lightVec);

    float diffuse  = max(dot(_normal, lightDir), 0.0);
    float specular = calculateBlinnSpecularCoefficient(cameraDir, lightDir, _normal);
    float attenuation = 1.0 / (1.0 + light.linear * _distance + light.quadratic * _distance * _distance);

    vec3 lit = (colors.specular * specular + colors.diffuse * diffuse);

    return  (lit + colors.ambient) * attenuation;
}

LightColors getLightingColors(LightColors lightColors) {
    LightColors result;

    float specular = texture(gAlbedoSpec, texCoords).a;
    vec3 diffuse = texture(gAlbedoSpec, texCoords).rgb;

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
