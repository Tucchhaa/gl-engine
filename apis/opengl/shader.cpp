#include "shader.hpp"

Shader::Shader(string vertexShaderFile, string fragmentShaderFile) {
    this->ID = this->createShader(vertexShaderFile, fragmentShaderFile);
}

void Shader::use() {
    glUseProgram(ID);
}

void Shader::deleteShader() {
    glDeleteProgram(ID);
}

int Shader::getLocation(const string name) const {
    return glGetUniformLocation(ID, name.c_str());
}

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

void Shader::setMaterial(const Material* material) const {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, material->diffuseTextures[0]);
    setInt("material.diffuse", 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, material->specularTextures[0]);
    setInt("material.specular", 1);
    
    setFloat("material.shininess", 32);
}

void Shader::setDirectLight(uint index, const DirectLight* lightSource) const {
    string _index = to_string(index);
    
    Transform* transform = Hierarchy::getTransform(lightSource->GameObjectID);
    
    setVec3(("directLights[" + _index + "].direction").c_str(), transform->getDirectionVector());
    
    setVec3(("directLights[" + _index + "].colors.ambient").c_str(),  lightSource->ambient);
    setVec3(("directLights[" + _index + "].colors.diffuse").c_str(),  lightSource->diffuse);
    setVec3(("directLights[" + _index + "].colors.specular").c_str(), lightSource->specular);
}

void Shader::setPointLight(uint index, const PointLight* lightSource) const {
    string _index = to_string(index);
    
    Transform* transform = Hierarchy::getTransform(lightSource->GameObjectID);
    
    setVec3(("pointLights[" + _index + "].position").c_str(), transform->position);
    
    setFloat(("pointLights[" + _index + "].linear").c_str(), lightSource->linear);
    setFloat(("pointLights[" + _index + "].quadratic").c_str(), lightSource->quadratic);
    
    setVec3(("pointLights[" + _index + "].colors.ambient").c_str(),  lightSource->ambient);
    setVec3(("pointLights[" + _index + "].colors.diffuse").c_str(),  lightSource->diffuse);
    setVec3(("pointLights[" + _index + "].colors.specular").c_str(), lightSource->specular);
}

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

uint Shader::createShader(string& verterxShaderFile, string& fragmentShaderFile) {
    uint shaderProgramID = glCreateProgram();
    
    const string path = "/Users/tucha/Repositories/gl-engine/gl-engine/apis/opengl/shaders";
    
    string vertexCode = readShader(path + "/" + verterxShaderFile);
    string fragmentCode = readShader(path + "/" + fragmentShaderFile);
    
    uint vertexShaderID = compileShader(GL_VERTEX_SHADER, vertexCode);
    uint fragmentShaderID = compileShader(GL_FRAGMENT_SHADER, fragmentCode);
    
    glAttachShader(shaderProgramID, vertexShaderID);
    glAttachShader(shaderProgramID, fragmentShaderID);
    
    glLinkProgram(shaderProgramID);
    glValidateProgram(shaderProgramID);
    
    int result;
    glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &result);
    
    if(result == GL_FALSE) {
        char message[512];
        glGetProgramInfoLog(ID, 512, NULL, message);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << message << std::endl;
    }
    
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
    
    return shaderProgramID;
}
