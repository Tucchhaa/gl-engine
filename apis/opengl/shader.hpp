#pragma once

#define GL_SILENCE_DEPRECATION

#include <OpenGL/gl3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <iostream>
#include <string>

#include "resource-manager.hpp"

#include "../../core/hierarchy.hpp"

#include "../../core/game-object/components/mesh.hpp"
#include "../../core/game-object/components/lights/direct-light.hpp"
#include "../../core/game-object/components/lights/point-light.hpp"
#include "../../core/game-object/components/lights/spot-light.hpp"

using namespace std;
using namespace glm;

using uint = unsigned int;

class Shader {
public:
    uint ID;

    Shader(
            const string& vertexShaderFile, const string& fragmentShaderFile,
            const string& tessControlShaderFile = "", const string& tessEvalShaderFile = "");

// ===
// Basic methods
// ===
public:
    void use();

    void deleteShader() const;

    int getLocation(const string& name) const;

private:
    int texturesCount = 0;

// ==
// Primitive setters
// ===
public:
    void setBool(const string &name, bool value) const;
    void setInt(const string &name, int value) const;
    void setFloat(const string &name, float value) const;
    
    void setMat4(const string& name, const mat4& matrix) const;
    void setMat3(const string& name, const mat3& matrix) const;
    
    void setVec3(const string& name, const vec3& vector) const;

public:
    // ===
    // Material
    // ===

    void setTexture(const string& name, unsigned int textureId);
    void setTexture(const string& name, unsigned int textureId, int slot);

    void setMaterial(const Material* material);

    // ==
    // Light components
    // ===

    void setDirectLight(uint index, const DirectLight* lightSource) const;
    void setPointLight(uint index, const PointLight* lightSource) const;
    void setSpotLight(uint index, const SpotLight* lightSource) const;

// ===
// Shader initialization
// ===
private:
    string readShader(const string& filepath);
    uint compileShader(uint type, string& code);
    uint createShaderProgram(vector<const string*> shaderFiles);
};
