#version 410 core

#define DIRECT_LIGHTS_LENGTH 5
#define POINT_LIGHTS_LENGTH 5
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

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
    float shininess;
};

uniform PointLight pointLights[POINT_LIGHTS_LENGTH];

uniform Material material;
uniform sampler2D shadowMap;

uniform vec3 cameraPos;

in struct OUTPUT {
    vec3 fragPos;
    vec4 fragPosLightSpace;
    vec3 normal;
    vec3 tangent;
    vec2 texCoord;
} data;

out vec4 color;

vec3 calculatePointLight(PointLight lightSource, vec3 cameraDir, vec3 c_surface);

void main() {
    vec3 cameraDir = normalize(cameraPos - data.fragPos);

    vec3 c_surface = texture(material.diffuse, data.texCoord).rgb;
    vec3 c_cool = vec3(0.0, 0.0, 0.55) + 0.25 * c_surface;
    vec3 result = c_cool * 0.5;

    for(int i=0; i < POINT_LIGHTS_LENGTH; i++) {
        result += calculatePointLight(pointLights[i], cameraDir, c_surface);
    }
    
    color = vec4(result, 1);
}

vec3 calculatePointLight(PointLight light, vec3 cameraDir, vec3 c_surface) {
    vec3 normal = normalize(data.normal);
    vec3 position = data.fragPos;

    vec3 lightVec = light.position - position;
    float _distance = length(lightVec);
    float attenuation = 1.0 / (1.0 + light.linear * _distance + light.quadratic * _distance * _distance);

    vec3 lightDir = normalize(lightVec);

    vec3 c_warm = vec3(0.3, 0.3, 0.0) + 0.25 * c_surface;
    vec3 c_highlight = vec3(2.0, 2.0, 2.0);

    vec3 r = 2*dot(normal, lightDir) * normal - lightDir;
    vec3 s = clamp(100 * dot(r, cameraDir) * r - 97, 0.0, 1.0);

    vec3 lit = mix(c_warm, c_highlight, s);

    vec3 result = max(dot(lightDir, normal), 0.0) * light.colors.diffuse * lit * attenuation;

    return result;
}
