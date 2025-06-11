#pragma once



class Scene {

public:
    virtual void Init() = 0;
    virtual void Update(float deltatime) = 0;
    virtual void Render() = 0;
    virtual void Shutdown() = 0;
protected:
    

};