#pragma once

#include "../base/iengine.hpp"

class GlEngine: public IEngine {
public:
    GlEngine();

    ~GlEngine() override;

private:
    void init();
};