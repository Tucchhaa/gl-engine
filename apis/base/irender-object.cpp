#include "irender-object.hpp"

#include "../../core/components/mesh.hpp"
#include "../../core/components/cubic-patch.hpp"
#include "../../core/components/terrain.hpp"

template<typename T>
T* IRenderObject::getMesh() {
    return dynamic_cast<T*>(mesh);
}

template Mesh* IRenderObject::getMesh<Mesh>();
template CubicPatch* IRenderObject::getMesh<CubicPatch>();
template Terrain* IRenderObject::getMesh<Terrain>();
