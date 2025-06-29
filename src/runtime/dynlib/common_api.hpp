#pragma once

#include <engine/scene.hpp>


#define ENGINE_LIB_NAME "engine"

enum class EngineFunctions {
    EngineInit,
    GetScene,
};

struct EngineApi {
    // Engine Functions
    void (*EngineInit)();

    // Scene Functions
    bool (*RegisterScene)(Scene_API*);
    Scene_API* (*GetScene)(const char*);
    std::vector<const char*> (*GetSceneNames)();
};
#define ENGINE_API_SIZE sizeof(EngineApi) / sizeof(void*)

struct EngineLibrary {
    void*       handle = nullptr;
    bool        is_fully_loaded = false;
    EngineApi   api;
};
void isEngineLoadSuccess(EngineLibrary& lib);
bool DidLoadSucceed(int funcc, const void* funcv[]);