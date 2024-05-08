#include "shader.hpp"
#include "gl-check-error.hpp"

#include <iostream>

#include "resource-manager.hpp"

Shader::Shader(
        const string& vertexShaderFile,
        const string& fragmentShaderFile,
        const string& tessControlShaderFile,
        const string& tessEvalShaderFile
        ) {
    vector<const string*> shaders = {
        &vertexShaderFile,
        &fragmentShaderFile,
        &tessControlShaderFile,
        &tessEvalShaderFile
    };

    this->ID = this->createShaderProgram(shaders);
}

// ===
// Basic methods
// ===

void Shader::use() {
    glUseProgram(ID);

    texturesCount = 5; // these first slots are reserved for material textures
}

void Shader::deleteShader() const {
    glDeleteProgram(ID);
}

// ==
// Primitive setters
// ===

void Shader::setBool(const string &name, bool value)  {
    glUniform1i(getLocation(name), (int)value);
}

void Shader::setInt(const string &name, int value) {
    glUniform1i(getLocation(name), value);
}

void Shader::setFloat(const string &name, float value)  {
    glUniform1f(getLocation(name), value);
}

void Shader::setMat4(const string& name, const mat4& matrix)  {
    glUniformMatrix4fv(getLocation(name), 1, GL_FALSE, value_ptr(matrix));
}

void Shader::setMat3(const string& name, const mat3& matrix)  {
    glUniformMatrix3fv(getLocation(name), 1, GL_FALSE, value_ptr(matrix));
}

void Shader::setVec3(const string& name, const Vec3& vector) {
    glUniform3f(getLocation(name), vector.x, vector.y, vector.z);
}

void Shader::setVec3(const string& name, const vec3& vector)  {
    glUniform3f(getLocation(name), vector.x, vector.y, vector.z);
}

// ===
// Textures
// ===

/**
 * Sets texture to the next available slot.
 * @param name uniform name of the texture
 * @param textureId ID generated by OpenGL
 * @param type texture type. GL_TEXTURE_2D by default.
 */
void Shader::setTexture(const string& name, const unsigned int textureId, const unsigned int type) {
    setTextureToSlot(name, textureId, texturesCount, type);

    texturesCount++;
}

/**
 * Sets texture to the dedicated slot.
 * @param name uniform name of the texture
 * @param textureId ID generated by OpenGL
 * @param slot texture slot.
 * @param type texture type. GL_TEXTURE_2D by default.
 */
void Shader::setTextureToSlot(const string& name, const unsigned int textureId, const int slot, const unsigned int type) {
    const int textureSlot = GL_TEXTURE0 + slot;

    glActiveTexture(textureSlot);
    glBindTexture(type, textureId);
    setInt(name, slot);
}


// ===
// Material
// ===

/**
 * Sets material textures to the reserved slots.
 * Reserved slots are used to assign material's of different objects to the same slots.
 * @param material
 */
void Shader::setMaterial(const Material* material) {
    setTextureToSlot("material.diffuse", ResourceManager::getTextureId(material, TEXTURE_DIFFUSE), 0);
    setTextureToSlot("material.specular", ResourceManager::getTextureId(material, TEXTURE_SPECULAR), 1);
    setTextureToSlot("material.normal", ResourceManager::getTextureId(material, TEXTURE_NORMAL), 2);
    setTextureToSlot("material.roughness", ResourceManager::getTextureId(material, TEXTURE_ROUGHNESS), 3);
    setTextureToSlot("material.ao", ResourceManager::getTextureId(material, TEXTURE_AO), 4);
    
    setFloat("material.shininess", 32);
    setVec3("material.Kd", material->Kd);
    setVec3("material.Ks", material->Ks);
}

// ==
// Light components
// ===

void Shader::setDirectLight(uint index, const DirectLight* lightSource) {
    string _index = to_string(index);
    
    setVec3("directLights[" + _index + "].direction", lightSource->transform->getDirectionVector());
    
    setVec3("directLights[" + _index + "].colors.ambient",  lightSource->ambient);
    setVec3("directLights[" + _index + "].colors.diffuse",  lightSource->diffuse);
    setVec3("directLights[" + _index + "].colors.specular", lightSource->specular);
}

void Shader::setPointLight(const string& name, const PointLight* lightSource) {
    setVec3(name + ".position", lightSource->transform->getAbsolutePosition());

    setFloat(name + ".linear", lightSource->linear);
    setFloat(name + ".quadratic", lightSource->quadratic);

    setVec3(name + ".colors.ambient",  lightSource->ambient);
    setVec3(name + ".colors.diffuse",  lightSource->diffuse);
    setVec3(name + ".colors.specular", lightSource->specular);
}

void Shader::setSpotLight(uint index, const SpotLight* lightSource) {
    string _index = to_string(index);

    Transform* transform = lightSource->transform;

    setVec3("spotLights[" + _index + "].position", transform->getAbsolutePosition());
    setVec3("spotLights[" + _index + "].direction", transform->getDirectionVector());

    setFloat("spotLights[" + _index + "].coneAngleCosine", glm::cos(lightSource->coneAngle));
    setFloat("spotLights[" + _index + "].edgeAngleCosine", glm::cos(lightSource->coneAngle + lightSource->outerAngle));

    setFloat("spotLights[" + _index + "].linear", lightSource->linear);
    setFloat("spotLights[" + _index + "].quadratic", lightSource->quadratic);

    setVec3("spotLights[" + _index + "].colors.ambient",  lightSource->ambient);
    setVec3("spotLights[" + _index + "].colors.diffuse",  lightSource->diffuse);
    setVec3("spotLights[" + _index + "].colors.specular", lightSource->specular);
}

// ===
// Shader initialization
// ===

string Shader::readShader(const string& filepath) {
    std::ifstream stream(filepath);

    string line;
    string result = "";
    
    while(getline(stream, line)) {
        result += line + "\n";
    }
    
    stream.close();
    
    return result;
}

uint Shader::compileShader(uint type, string& code) {
    uint shaderId = glCreateShader(type);
    
    const char* src = code.c_str();
    
    glShaderSource(shaderId, 1, &src, NULL);
    glCompileShader(shaderId);
    
    int result;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
    
    if(result == GL_FALSE) {
        char message[512];
        glGetShaderInfoLog(shaderId, 512, NULL, message);
        
        string shaderName = type == GL_VERTEX_SHADER ? "vertex" : "fragment";
        std::cout << "Failed to compile " << shaderName << " shader: " << message << std::endl;
    }
    
    return shaderId;
}

uint Shader::createShaderProgram(vector<const string*> shaderFiles) {
    uint programId = glCreateProgram();
    vector<uint> shaderIds;

    const string SHADERS_DIR = "/Users/tucha/Repositories/gl-engine/apis/opengl/shaders";

    for(const string* path: shaderFiles) {
        if(path->empty())
            continue;

        string shaderCode = readShader(SHADERS_DIR + "/" + *path);

        string extension = path->substr(path->rfind('.')+1);

        uint shaderType;

        if(extension == "vert") shaderType = GL_VERTEX_SHADER;
        else if(extension == "frag") shaderType = GL_FRAGMENT_SHADER;
        else if(extension == "tesc") shaderType = GL_TESS_CONTROL_SHADER;
        else if(extension == "tese") shaderType = GL_TESS_EVALUATION_SHADER;
        else throw std::runtime_error("unsupported shader extension");

        uint shaderId = compileShader(shaderType, shaderCode);

        glAttachShader(programId, shaderId);

        shaderIds.push_back(shaderId);
    }
    
    glLinkProgram(programId);
    glValidateProgram(programId);
    
    int result;
    glGetProgramiv(programId, GL_LINK_STATUS, &result);
    
    if(result == GL_FALSE) {
        char message[512];
        glGetProgramInfoLog(programId, 512, NULL, message);

        throw std::runtime_error("ERROR::SHADER::PROGRAM::LINKING_FAILED\n" + string(message) + "\n");
    }

    for(uint shaderId: shaderIds) {
        glDeleteShader(shaderId);
    }

    return programId;
}

// ===
// Private methods
// ===

int Shader::getLocation(const string& name) {
    for(auto &[fst, snd] : cachedLocations) {
        if(fst == name) {
            return snd;
        }
    }

    const int location = glGetUniformLocation(ID, name.c_str());

    cachedLocations.emplace_back( name, location );

    return location;
}