#pragma once

#include "../../core/imesh.hpp"

#include "../../core/components/mesh.hpp"
#include "../../core/components/cubic-patch.hpp"
#include "../../core/components/terrain.hpp"

class IRenderObject {
protected:
    IMesh* mesh;

public:
    explicit IRenderObject(IMesh* mesh) : mesh(mesh) {};

    virtual ~IRenderObject() = default;

    virtual void render() = 0;

    template<typename T>
    T* getMesh() {
        return dynamic_cast<T*>(mesh);
    }
};

template Mesh* IRenderObject::getMesh<Mesh>();
template CubicPatch* IRenderObject::getMesh<CubicPatch>();
template Terrain* IRenderObject::getMesh<Terrain>();
