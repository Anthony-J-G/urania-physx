#pragma once

#include "engine/macros.h"
#include <vector>

#ifdef __cplusplus
extern "C" {
#endif

//! main structure for clients to use for interfacing with the engine. 
struct Scene_API {
    const char* name = nullptr;
    void (*initialize_fn)();
    void (*shutdown_fn)();
    void (*update_fn)(float);
    void (*render_fn)();
};


ENGINE_API bool RegisterScene(Scene_API(*fn)(void));
ENGINE_API Scene_API* GetScene(const char* key);

#ifdef __cplusplus
}
#endif



ENGINE_API std::vector<const char*> GetSceneNames();


// TODO(anthony-j-g): Remove legacy scene class to be replaced with Scene_API struct

class Scene {

public:
    virtual void Init() = 0;
    virtual void Update(float deltatime) = 0;
    virtual void Render() = 0;
    virtual void Shutdown() = 0;
protected:

};
