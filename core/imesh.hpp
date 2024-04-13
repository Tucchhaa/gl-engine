#pragma once

#include <vector>

class IMesh {
public:
    virtual ~IMesh() = default;

    // === Required ===

    virtual void* data() = 0;

    virtual int dataSize() = 0;

    virtual std::vector<int> shaderAttributes() = 0;

    // === Index buffer ===

    virtual bool usesIndices() {
        return false;
    }

    virtual void* indicesData() {
        throw std::runtime_error("Not implemented");
    };

    virtual int indicesDataSize() {
        throw std::runtime_error("Not implemented");
    }

    // === Tessellation ===

    virtual bool usesTessellation() {
        return false;
    }

    virtual int getVertexCount() {
        throw std::runtime_error("Not implemented");
    };

    virtual int getVerticesPerPatch() {
        throw std::runtime_error("Not implemented");
    };
};