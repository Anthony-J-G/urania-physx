#pragma once


// #ifdef __cplusplus
//     extern "C" {
// #endif

//! main structure for clients to use for interfacing with the engine. 
struct Scene_API {
    const char* name;
    void (*initialize_fn)();
    void (*update_fn)(float);
    void (*render_fn)();
};

class Scene {

public:
    virtual void Init() = 0;
    virtual void Update(float deltatime) = 0;
    virtual void Render() = 0;
    virtual void Shutdown() = 0;
protected:    

};

bool RegisterScene(Scene_API* scene);
Scene_API* GetScene(const char* key);

// #ifdef __cplusplus
//     }
// #endif    