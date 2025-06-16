#pragma once


//! main structure for clients to use for interfacing with the engine. 
struct Scene_API {
    void (*initialize_fn)();
    void (*update_fn)(float);
    void (*initialize_fn)();
};


class Scene {

public:
    virtual void Init() = 0;
    virtual void Update(float deltatime) = 0;
    virtual void Render() = 0;
    virtual void Shutdown() = 0;
protected:
    

};