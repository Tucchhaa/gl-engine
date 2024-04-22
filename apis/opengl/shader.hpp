#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <fstream>
#include <string>

#include "../../core/hierarchy.hpp"

#include "../../core/components/mesh.hpp"
#include "../../core/components/lights/direct-light.hpp"
#include "../../core/components/lights/point-light.hpp"
#include "../../core/components/lights/spot-light.hpp"

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

private:
    int texturesCount = 0;

    /**
     * First: Slot, second: texutreId
     */
    vector<pair<unsigned int, unsigned int>> reservedTextureSlots;

    /**
     * First: uniform name, second: location
     */
    vector<pair<string, int>> cachedLocations;

// ==
// Primitive setters
// ===
public:
    void setBool(const string &name, bool value);
    void setInt(const string &name, int value);
    void setFloat(const string &name, float value);
    
    void setMat4(const string& name, const mat4& matrix);
    void setMat3(const string& name, const mat3& matrix);
    
    void setVec3(const string& name, const vec3& vector);

public:
    // ===
    // Textures
    // ===

    void setTexture(const string& name, unsigned int textureId);
    void setTexture(const string& name, unsigned int textureId, int slot);

    // ===
    // Material
    // ===

    void setMaterial(const Material* material);

    // ==
    // Light components
    // ===

    void setDirectLight(uint index, const DirectLight* lightSource);

    void setPointLight(const string& name, const PointLight* lightSource);

    void setSpotLight(uint index, const SpotLight* lightSource);

// ===
// Shader initialization
// ===
private:
    string readShader(const string& filepath);
    uint compileShader(uint type, string& code);
    uint createShaderProgram(vector<const string*> shaderFiles);

// ===
// Private methods
// ===
private:
    int getLocation(const string& name);
};
