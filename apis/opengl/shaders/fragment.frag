#version 330 core

#define DIRECT_LIGHTS_LENGTH 5
#define POINT_LIGHTS_LENGTH 5
#define SPOT_LIGHTS_LENGTH 5

struct LightColors {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct DirectLight {
    vec3 direction;
    
    LightColors colors;
};

struct PointLight {
    vec3 position;
    
    float linear;
    float quadratic;
    
    LightColors colors;
};

struct SpotLight {
    vec3 position;
    vec3 direction;

    float coneAngleCosine;
    float edgeAngleCosine;

    float linear;
    float quadratic;

    LightColors colors;
};

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

uniform DirectLight directLights[DIRECT_LIGHTS_LENGTH];
uniform PointLight pointLights[POINT_LIGHTS_LENGTH];
uniform SpotLight spotLights[SPOT_LIGHTS_LENGTH];

uniform Material material;

uniform vec3 cameraPos;

in vec3 fragPos;
in vec3 normal;
in vec2 texCoord;

out vec4 color;

vec3 calculateDirectLight(DirectLight lightSource, vec3 _normal, vec3 cameraDir);
vec3 calculatePointLight(PointLight lightSource, vec3 _normal, vec3 position, vec3 cameraDir);
vec3 calculateSpotLight(SpotLight light, vec3 _normal, vec3 position, vec3 cameraDir);

LightColors getLightingColors(LightColors lightColors);
float calculateBlinnSpecularCoefficient(vec3 cameraDir, vec3 lightDir, vec3 normal);

void main() {
    vec3 _normal = normalize(normal);
    vec3 cameraDir = normalize(cameraPos - fragPos);
    
    vec3 result = vec3(0, 0, 0);
    
    for(int i=0; i < DIRECT_LIGHTS_LENGTH; i++) {
        result += calculateDirectLight(directLights[i], _normal, cameraDir);
    }
    
    for(int i=0; i < POINT_LIGHTS_LENGTH; i++) {
        result += calculatePointLight(pointLights[i], _normal, fragPos, cameraDir);
    }

    for(int i=0; i < SPOT_LIGHTS_LENGTH; i++) {
        result += calculateSpotLight(spotLights[i], _normal, fragPos, cameraDir);
    }
    
    color = vec4(result, 1);
//    color = texture(material.diffuse, texCoord);
//    color = vec4(_normal, 1);
}

vec3 calculateDirectLight(DirectLight light, vec3 _normal, vec3 cameraDir) {
    LightColors colors = getLightingColors(light.colors);
    
    float diffuse  = max(dot(_normal, -light.direction), 0.0);
    float specular = calculateBlinnSpecularCoefficient(cameraDir, -light.direction, _normal);
    
    return  
             colors.specular * specular +
             colors.diffuse  * diffuse +
             colors.ambient;
}

vec3 calculatePointLight(PointLight light, vec3 _normal, vec3 position, vec3 cameraDir) {
    LightColors colors = getLightingColors(light.colors);
    
    vec3 lightVec = light.position - position;
    float _distance = length(lightVec);
    
    vec3 lightDir = normalize(lightVec);
    
    float diffuse  = max(dot(_normal, lightDir), 0.0);
    float specular = calculateBlinnSpecularCoefficient(cameraDir, lightDir, _normal);
    float attenuation = 1.0 / (1.0 + light.linear * _distance + light.quadratic * _distance * _distance);
    
    return  (
             colors.specular * specular +
             colors.diffuse  * diffuse +
             colors.ambient) * attenuation;
}

vec3 calculateSpotLight(SpotLight light, vec3 _normal, vec3 position, vec3 cameraDir) {
    LightColors colors = getLightingColors(light.colors);

    vec3 lightVec = light.position - position;
    float _distance = length(lightVec);

    vec3 lightDir = normalize(lightVec);
    // alpha is angle between light direction and direction to fragment
    float cosTheta = dot(lightDir, light.direction);
    float intensity = clamp((cosTheta - light.edgeAngleCosine) / (light.coneAngleCosine - light.edgeAngleCosine), 0.0, 1.0);

    float diffuse  = max(dot(_normal, lightDir), 0.0);
    float specular = calculateBlinnSpecularCoefficient(cameraDir, lightDir, _normal);
    float attenuation = 1.0 / (1.0 + light.linear * _distance + light.quadratic * _distance * _distance);

    return  (
        colors.specular * specular +
        colors.diffuse  * diffuse) * attenuation * intensity +
        colors.ambient * attenuation;
}

LightColors getLightingColors(LightColors lightColors) {
    LightColors result;
    
    result.specular = vec3(texture(material.specular, texCoord)) * lightColors.specular;
    result.diffuse  = vec3(texture(material.diffuse,  texCoord)) * lightColors.diffuse;
    result.ambient  = result.diffuse * lightColors.ambient;
    
    return result;
}

float calculateBlinnSpecularCoefficient(vec3 cameraDir, vec3 lightDir, vec3 normal) {
    vec3 halfwayDir = normalize(cameraDir + lightDir);
    
    return pow(max(dot(normal, halfwayDir), 0.0), material.shininess * 3);
}

//float calculateSpecularCoefficient(vec3 cameraDir, vec3 reflectedDir) {
//    return pow(max(dot(cameraDir, reflectedDir), 0.0), material.shininess);
//}
