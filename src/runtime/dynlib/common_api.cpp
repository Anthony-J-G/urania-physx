#include "common_api.hpp"



void isEngineLoadSuccess(EngineLibrary &lib) {
    lib.is_fully_loaded = (
        lib.api.EngineInit != nullptr && lib.api.RegisterScene != nullptr &&
        lib.api.GetScene != nullptr && lib.api.GetSceneNames != nullptr
    );
}
