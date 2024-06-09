//
// Created by Eldar Iusupzhanov on 2024/5/6.
//

#include "material-info.hpp"

#include <iostream>
#include <map>

MaterialInfo::MaterialInfo(const aiMaterial* material): material(material) {
    collectInfo();
}

void MaterialInfo::collectInfo() {
    name = getProperty<string>(AI_MATKEY_NAME);
    diffuseColor = getProperty<Vec3>(AI_MATKEY_COLOR_DIFFUSE);
    specularColor = getProperty<Vec3>(AI_MATKEY_COLOR_SPECULAR);
    ambientColor = getProperty<Vec3>(AI_MATKEY_COLOR_AMBIENT);
    emissiveColor = getProperty<Vec3>(AI_MATKEY_COLOR_EMISSIVE);
    reflectiveColor = getProperty<Vec3>(AI_MATKEY_COLOR_REFLECTIVE);

    collectTextures();
}

template<>
string MaterialInfo::getProperty(const char* key, unsigned int type, unsigned int idx) {
    if(aiString str; material->Get(key, type, idx, str) == AI_SUCCESS) {
        return {str.C_Str()};
    }
}

template<>
Vec3 MaterialInfo::getProperty(const char* key, unsigned int type, unsigned int idx) {
    if(aiColor4D color; material->Get(key, type, idx, color) == AI_SUCCESS) {
        return Vec3(color.r, color.g, color.b);
    }
}

bool MaterialInfo::operator==(const MaterialInfo& other) const {
    if(textures.size() != other.textures.size())
        return false;

    for(auto [key, texturePath] : textures) {
        auto otherIt = other.textures.find(key);

        if(otherIt == other.textures.end() || otherIt->second != texturePath)
            return false;
    }

    return
        diffuseColor == other.diffuseColor &&
        specularColor == other.specularColor &&
        ambientColor == other.ambientColor &&
        emissiveColor == other.emissiveColor &&
        reflectiveColor == other.reflectiveColor;
}

bool MaterialInfo::operator<(const MaterialInfo& other) const {
    if(textures.size() < other.textures.size())
        return true;

    for(int type=aiTextureType_NONE; type <= aiTextureType_TRANSMISSION; type++) {
        auto it1 = textures.find(static_cast<aiTextureType>(type));
        auto it2 = other.textures.find(static_cast<aiTextureType>(type));

        if(it1 == textures.end() && it2 != other.textures.end())
            return true;
    }

    return
        diffuseColor < other.diffuseColor ||
        specularColor < other.specularColor ||
        ambientColor < other.ambientColor ||
        emissiveColor < other.emissiveColor ||
        reflectiveColor < other.reflectiveColor;
}

void MaterialInfo::collectTextures() {
    for(int type=aiTextureType_NONE; type <= aiTextureType_TRANSMISSION; type++) {
        auto aiType = static_cast<aiTextureType>(type);
        const unsigned int count = material->GetTextureCount(aiType);

        if(count > 0) {
            aiString path;

            if(material->GetTexture(aiType, 0, &path) == AI_SUCCESS) {
                textures[aiType] = path.C_Str();
            }
        }
    }
}

void MaterialInfo::print() {
    std::cout << "= Material Info =" << std::endl;

    // output properties
    printProperty<string>("Name", AI_MATKEY_NAME);
    printProperty<Vec3>("Diffuse color", AI_MATKEY_COLOR_DIFFUSE);
    printProperty<Vec3>("Specular color", AI_MATKEY_COLOR_SPECULAR);
    printProperty<Vec3>("Ambient color", AI_MATKEY_COLOR_AMBIENT);
    printProperty<Vec3>("Emissive color", AI_MATKEY_COLOR_EMISSIVE);
    printProperty<Vec3>("Reflective color", AI_MATKEY_COLOR_REFLECTIVE);

    printTextures();
}

void MaterialInfo::printTextures() {
    map<aiTextureType, string> typesName = {
        { aiTextureType_NONE, "NONE" },
        { aiTextureType_DIFFUSE, "DIFFUSE" },
        { aiTextureType_SPECULAR, "SPECULAR" },
        { aiTextureType_AMBIENT, "AMBIENT" },
        { aiTextureType_EMISSIVE, "EMISSIVE" },
        { aiTextureType_HEIGHT, "HEIGHT" },
        { aiTextureType_NORMALS, "NORMALS" },
        { aiTextureType_SHININESS, "SHININESS" },
        { aiTextureType_OPACITY, "OPACITY" },
        { aiTextureType_DISPLACEMENT, "DISPLACEMENT" },
        { aiTextureType_LIGHTMAP, "LIGHTMAP" },
        { aiTextureType_REFLECTION, "REFLECTION" },
        { aiTextureType_BASE_COLOR, "BASE_COLOR" },
        { aiTextureType_NORMAL_CAMERA, "NORMAL_CAMERA" },
        { aiTextureType_EMISSION_COLOR, "EMISSION_COLOR" },
        { aiTextureType_METALNESS, "METALNESS" },
        { aiTextureType_DIFFUSE_ROUGHNESS, "DIFFUSE_ROUGHNESS" },
        { aiTextureType_AMBIENT_OCCLUSION, "AMBIENT_OCCLUSION" },
        { aiTextureType_UNKNOWN, "UNKNOWN" },
        { aiTextureType_TRANSMISSION, "TRANSMISSION" }
    };

    for(int type=aiTextureType_NONE; type <= aiTextureType_TRANSMISSION; type++) {
        auto aiType = static_cast<aiTextureType>(type);

        if(auto it = textures.find(aiType); it != textures.end()) {
            string typeName = typesName[aiType];
            unsigned int count = material->GetTextureCount(aiType);

            std::cout << "Texture type: " << typeName << ", count: " << count << ": ";

            for(unsigned int i = 0; i < count; ++i) {
                aiString path;

                if(material->GetTexture(aiType, i, &path) == AI_SUCCESS) {
                    std::cout << path.C_Str() << ", ";
                }
            }

            std::cout << std::endl;
        }
    }
}

template<typename T>
void MaterialInfo::printProperty(
    const string &propertyName, const char* key,
    const unsigned int type, const unsigned int idx
) const {
    const bool isString = typeid(T) == typeid(string);
    const bool isColor  = typeid(T) == typeid(Vec3);

    cout << "Property: " << propertyName << ": ";
    if(aiString str; isString && material->Get(key, type, idx, str) == AI_SUCCESS) {
        std::cout << str.C_Str();
    }
    else if(aiColor4D color; isColor && material->Get(key, type, idx, color) == AI_SUCCESS) {
        std::cout << color.r << " " << color.g << " " << color.b;
    }
    else {
        std::cout << "Can not get property";
    }

    cout << std::endl;
}
