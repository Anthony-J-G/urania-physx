#pragma once



#include "physics/scene.hpp"


struct PhysicsLibrary {
    bool is_loaded;
    void (*EngineInit)();
    Scene* (*GetScene)(const char*);
    void (*Scene__Init)();
    void (*Scene__Update)(float);
    void (*Scene__Render)();
    void (*Scene__Shutdown)();
    int load_scene_from_list;
};