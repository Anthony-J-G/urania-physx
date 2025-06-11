#pragma once



class OscillatingCircleScene {
public:
    OscillatingCircleScene();

    void Init();
    void Update(float deltatime);
    void Render();


private:
    bool draw_grid = true;
};