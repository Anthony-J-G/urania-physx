#pragma once

#include "scene.hpp"


class OscillatingCircleScene : public Scene {
public:
    OscillatingCircleScene();

    void Init();
    void Update(float deltatime);
    void Render();
    void Shutdown();

private:
    bool draw_grid = true;
};