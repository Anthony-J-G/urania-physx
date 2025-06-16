#pragma once

#include <physics/scene.hpp>


struct PhysicsLibrary {
    bool is_loaded;
    
    // Engine Functions
    void (*EngineInit)();

    // Scene Functions
    bool (*RegisterScene)(Scene_API*);
    Scene_API* (*GetScene)(const char*);
    std::vector<const char*> (*GetSceneNames)();
};