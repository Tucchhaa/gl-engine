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
LightColors getLightingColors(LightColors lightColors);
float calculateSpecularCoefficient(vec3 cameraDir, vec3 reflectedDir);

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
    
    color = vec4(result, 1);
//    color = texture(material.diffuse, texCoord);
//    color = vec4(_normal, 1);
}

vec3 calculateDirectLight(DirectLight light, vec3 _normal, vec3 cameraDir) {
    LightColors colors = getLightingColors(light.colors);
    
    vec3 reflectedDir = reflect(light.direction, _normal);
    
    float diffuse  = max(dot(_normal, -light.direction), 0.0);
    float specular = calculateSpecularCoefficient(cameraDir, reflectedDir);
    
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
    vec3 reflectedDir = reflect(-lightDir, _normal);
    
    float diffuse  = max(dot(_normal, lightDir), 0.0);
    float specular = calculateSpecularCoefficient(cameraDir, reflectedDir);
    float attenuation = 1.0 / (1.0 + light.linear * _distance + light.quadratic * _distance * _distance);
    
    return  (
             colors.specular * specular +
             colors.diffuse  * diffuse +
             colors.ambient) * attenuation;
}

LightColors getLightingColors(LightColors lightColors) {
    LightColors result;
    
    result.specular = vec3(texture(material.specular, texCoord)) * lightColors.specular;
    result.diffuse  = vec3(texture(material.diffuse,  texCoord)) * lightColors.diffuse;
    result.ambient  = result.diffuse * lightColors.ambient;
    
    return result;
}

float calculateSpecularCoefficient(vec3 cameraDir, vec3 reflectedDir) {
    return pow(max(dot(cameraDir, reflectedDir), 0.0), material.shininess);
}
