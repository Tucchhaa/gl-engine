#pragma once

#include "../core/scene.hpp"
#include "../apis/base/iinput.hpp"

class GoochShadingDemo : public Scene {
public:
    explicit GoochShadingDemo(Loader* loader);

    void setupScene() override;

    void beforeRender() override;

private:
    IInput* input;

    GameObject* lights;

    vector<pair<GameObject*, vec3>> lightTranslate;

    GameObject* createDragon();

    GameObject* createCube();
};
