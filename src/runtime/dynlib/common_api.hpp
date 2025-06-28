#pragma once

#include <engine/scene.hpp>


#define ENGINE_LIB_NAME "engine"


struct EngineLibrary {
    bool is_loaded;
    
    // Engine Functions
    void (*EngineInit)();

    // Scene Functions
    bool (*RegisterScene)(Scene_API*);
    Scene_API* (*GetScene)(const char*);
    std::vector<const char*> (*GetSceneNames)();
};
void isEngineLoadSuccess(EngineLibrary& api);
