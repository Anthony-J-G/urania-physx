#include "common_api.hpp"



void isEngineLoadSuccess(EngineLibrary &api) {
    api.is_loaded = (
        api.EngineInit != nullptr && api.RegisterScene != nullptr &&
        api.GetScene != nullptr && api.GetSceneNames != nullptr
    );
}
