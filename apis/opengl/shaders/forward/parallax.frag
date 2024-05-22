#version 410 core

#define POINT_LIGHTS_LENGTH 105

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
    sampler2D height;
    sampler2D specular;
    sampler2D normal;
    float shininess;
};

uniform PointLight pointLights[POINT_LIGHTS_LENGTH];

uniform Material material;

uniform float height_scale;

uniform vec3 cameraPos;

in struct OUTPUT {
    vec3 fragPos;
    vec3 normal;
    vec3 tangent;
    vec2 texCoord;

    vec3 viewDir;
} data;

out vec4 color;

vec2 parallaxMapping(vec3 tangentCameraDir);
vec3 calculatePointLight(PointLight lightSource, vec3 _normal, vec3 position, vec3 cameraDir, vec2 texCoord);
LightColors getLightingColors(LightColors lightColors, vec2 texCoord);
float calculateBlinnSpecularCoefficient(vec3 cameraDir, vec3 lightDir, vec3 normal);

void main() {
    vec3 bitangent = cross(data.normal, data.tangent);
    mat3 TBN = mat3(
        normalize(data.tangent),
        normalize(bitangent),
        normalize(data.normal)
    );

    mat3 TBN_inverse = transpose(TBN);

    vec3 cameraDir = normalize(cameraPos - data.fragPos);
    vec3 tangentCameraDir = TBN_inverse * cameraDir;
    vec2 texCoord = parallaxMapping(tangentCameraDir);

    if(texCoord.x > 1.0 || texCoord.y > 1.0 || texCoord.x < 0.0 || texCoord.y < 0.0)
        discard;

    vec3 _normal = texture(material.normal, texCoord).rgb;
    _normal = normalize(_normal * 2.0 - 1.0);
    _normal = normalize(TBN * _normal);

    vec3 result = vec3(0, 0, 0);

    for(int i=0; i < POINT_LIGHTS_LENGTH; i++) {
        result += calculatePointLight(pointLights[i], _normal, data.fragPos, cameraDir, texCoord);
    }

    color = vec4(result, 1);
}

vec2 parallaxMapping(vec3 tangentCameraDir) {
    const float minLayers = 8.0;
    const float maxLayers = 32.0;
    float numLayers = mix(maxLayers, minLayers, max(dot(vec3(0.0, 0.0, 1.0), tangentCameraDir), 0.0));

    float layerDepth = 1.0 / numLayers;
    float currentLayerDepth = 0.0;
    vec2 P = tangentCameraDir.xy * height_scale;
    vec2 deltaTexCoords = P / numLayers;

    vec2  currentTexCoords     = data.texCoord;
    float currentDepthMapValue = texture(material.height, currentTexCoords).r;

    while(currentLayerDepth < currentDepthMapValue)
    {
        currentTexCoords -= deltaTexCoords;
        currentDepthMapValue = texture(material.height, currentTexCoords).r;
        currentLayerDepth += layerDepth;
    }

    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    float afterDepth  = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = texture(material.height, prevTexCoords).r - currentLayerDepth + layerDepth;

    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords;
}

vec3 calculatePointLight(PointLight light, vec3 _normal, vec3 position, vec3 cameraDir, vec2 texCoord) {
    LightColors colors = getLightingColors(light.colors, texCoord);

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

    result.specular = vec3(texture(material.specular, texCoord)) * lightColors.specular;
    result.diffuse  = vec3(texture(material.diffuse,  texCoord)) * lightColors.diffuse;
    result.ambient  = result.diffuse * lightColors.ambient;

    return result;
}

float calculateBlinnSpecularCoefficient(vec3 cameraDir, vec3 lightDir, vec3 normal) {
    vec3 halfwayDir = normalize(cameraDir + lightDir);

    return pow(max(dot(normal, halfwayDir), 0.0), material.shininess * 3);
}
