#pragma once

#include "../core/scene.hpp"
#include "../apis/base/iinput.hpp"

class ManyLightsDemo: public Scene {
public:
    explicit ManyLightsDemo(Loader* loader);

    void setupScene() override;

    void beforeRender() override;

private:
    vector<pair<GameObject*, vec3>> lightTranslate;

    IInput* input;

    GameObject* createCube();
};


