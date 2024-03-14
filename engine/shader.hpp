#pragma once

#define GL_SILENCE_DEPRECATION

#include <OpenGL/gl3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <iostream>
#include <string>

#include "../game-object/hierarchy.hpp"
#include "../game-object/components/mesh.hpp"
#include "../game-object/components/lights/direct-light.hpp"
#include "../game-object/components/lights/point-light.hpp"

using namespace std;
using namespace glm;

using uint = unsigned int;

class Shader {
private:
    string readShader(const string& filepath);
    uint compileShader(uint type, string& code);
    uint createShader(string& verterxShaderFile, string& fragmentShaderFile);
    
public:
    uint ID;
    
    Shader(string vertexShaderFile, string fragmentShaderFile);
    
    void use();
    void deleteShader();
    
    // utility uniform functions
    int getLocation(const string name) const;
    
    void setBool(const string &name, const bool value) const;
    void setInt(const string &name, const int value) const;
    void setFloat(const string &name, const float value) const;
    
    void setMat4(const string& name, const mat4& matrix) const;
    void setMat3(const string& name, const mat3& matrix) const;
    
    void setVec3(const string& name, const vec3& vector) const;
    
    void setMaterial(const Material* material) const;
    
    void setDirectLight(uint index, const DirectLight* lightSource) const;
    void setPointLight(uint index, const PointLight* lightSource) const;
};
