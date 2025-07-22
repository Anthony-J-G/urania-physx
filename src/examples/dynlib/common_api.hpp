#pragma once

#include "runtime/macros.h"

#include <engine/scene.hpp>


#define ENGINE_LIB_NAME "engine"
#define GAME_LIB_NAME   "simulations"

#ifdef RUNTIME_OS_WINDOWS
#   define ENGINE_LIB_PATH ENGINE_LIB_NAME ".dll"
#   define GAME_LIB_PATH GAME_LIB_NAME ".dll"
#elif RUNTIME_OS_LINUX
#   define ENGINE_LIB_PATH ENGINE_LIB_NAME ".so"
#   define GAME_LIB_PATH GAME_LIB_NAME ".so"
#elif RUNTIME_OS_MACOS
#   define ENGINE_LIB_PATH ENGINE_LIB_NAME ".dynlib"
#   define GAME_LIB_PATH GAME_LIB_NAME ".dynlib"
#elif
#   error "No supported operating systems detected for dynamic linking"
#endif



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

extern void LoadEngineLibrary(EngineLibrary& lib);
extern void UnloadEngineLibrary(EngineLibrary& lib);


struct GameApi {
    // Engine Functions
    void (*EngineInit)();

    // Scene Functions
    bool (*RegisterScene)(Scene_API*);
    Scene_API* (*GetScene)(const char*);
    std::vector<const char*> (*GetSceneNames)();
};
#define ENGINE_API_SIZE sizeof(EngineApi) / sizeof(void*)

struct GameLibrary {
    void*       handle = nullptr;
    bool        is_fully_loaded = false;
    EngineApi   api;
};
void isEngineLoadSuccess(EngineLibrary& lib);
bool DidLoadSucceed(int funcc, const void* funcv[]);