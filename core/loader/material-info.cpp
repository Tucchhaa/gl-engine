//
// Created by Eldar Iusupzhanov on 2024/5/6.
//

#include "material-info.hpp"

#include <iostream>
#include <map>

void MaterialInfo::print(const aiMaterial* material) {
    std::cout << "= Material Info =" << std::endl;

    // output properties
    printProperty<aiString>(material, "Name", AI_MATKEY_NAME);
    printProperty<aiColor4D>(material, "Diffuse color", AI_MATKEY_COLOR_DIFFUSE);
    printProperty<aiColor4D>(material, "Specular color", AI_MATKEY_COLOR_SPECULAR);
    printProperty<aiColor4D>(material, "Ambient color", AI_MATKEY_COLOR_AMBIENT);
    printProperty<aiColor4D>(material, "Emissive color", AI_MATKEY_COLOR_EMISSIVE);
    printProperty<aiColor4D>(material, "Reflective color", AI_MATKEY_COLOR_REFLECTIVE);

    // output textures
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
        unsigned int count = material->GetTextureCount(aiType);

        if(count > 0) {
            string typeName = typesName[aiType];
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
    const aiMaterial* material, const string &propertyName, const char* key,
    const unsigned int type, const unsigned int idx
) {
    const bool isString = typeid(T) == typeid(aiString);
    const bool isColor  = typeid(T) == typeid(aiColor4D);

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
