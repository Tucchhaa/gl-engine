#include "shader.hpp"

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

void Shader::use() const {
    glUseProgram(ID);
}

void Shader::deleteShader() const {
    glDeleteProgram(ID);
}

int Shader::getLocation(const string& name) const {
    return glGetUniformLocation(ID, name.c_str());
}

// ==
// Primitive setters
// ===

void Shader::setBool(const string &name, bool value) const {
    glUniform1i(getLocation(name), (int)value);
}

void Shader::setInt(const string &name, int value) const {
    glUniform1i(getLocation(name), value);
}

void Shader::setFloat(const string &name, float value) const {
    glUniform1f(getLocation(name), value);
}

void Shader::setMat4(const string& name, const mat4& matrix) const {
    glUniformMatrix4fv(getLocation(name), 1, GL_FALSE, value_ptr(matrix));
}

void Shader::setMat3(const string& name, const mat3& matrix) const {
    glUniformMatrix3fv(getLocation(name), 1, GL_FALSE, value_ptr(matrix));
}

void Shader::setVec3(const string& name, const vec3& vector) const {
    glUniform3f(getLocation(name), vector.x, vector.y, vector.z);
}

// ===
// Material
// ===

void Shader::setMaterial(const Material* material) const {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ResourceManager::getTextureId(material, glApi_DIFFUSE_TEXTURE));
    setInt("material.diffuse", 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, ResourceManager::getTextureId(material, glApi_SPECULAR_TEXTURE));
    setInt("material.specular", 1);
    
    setFloat("material.shininess", 32);
}

// ==
// Light components
// ===

void Shader::setDirectLight(uint index, const DirectLight* lightSource) const {
    string _index = to_string(index);
    
    Transform* transform = Hierarchy::getTransform(lightSource->GameObjectID);
    
    setVec3("directLights[" + _index + "].direction", transform->getDirectionVector());
    
    setVec3("directLights[" + _index + "].colors.ambient",  lightSource->ambient);
    setVec3("directLights[" + _index + "].colors.diffuse",  lightSource->diffuse);
    setVec3("directLights[" + _index + "].colors.specular", lightSource->specular);
}

void Shader::setPointLight(uint index, const PointLight* lightSource) const {
    string _index = to_string(index);
    
    Transform* transform = Hierarchy::getTransform(lightSource->GameObjectID);
    
    setVec3("pointLights[" + _index + "].position", transform->getAbsolutePosition());
    
    setFloat("pointLights[" + _index + "].linear", lightSource->linear);
    setFloat("pointLights[" + _index + "].quadratic", lightSource->quadratic);
    
    setVec3("pointLights[" + _index + "].colors.ambient",  lightSource->ambient);
    setVec3("pointLights[" + _index + "].colors.diffuse",  lightSource->diffuse);
    setVec3("pointLights[" + _index + "].colors.specular", lightSource->specular);
}


void Shader::setSpotLight(uint index, const SpotLight* lightSource) const {
    string _index = to_string(index);

    Transform* transform = Hierarchy::getTransform(lightSource->GameObjectID);

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
