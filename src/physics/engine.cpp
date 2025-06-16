#include "engine.hpp"

#include <stdio.h>
#include <unordered_map>

#include "scene.hpp"
#include "scenes/sample.hpp"


using Map = std::unordered_map<const char*, Scene *>;

static Map g_SceneMapping;


void InitEngine() {
    printf("[physics] Engine initialized!");
    RegisterScene("OscillatingCircle", static_cast<Scene*>(new OscillatingCircleScene()));
}


void RegisterScene(const char* key, Scene* scene) {
    if (key == nullptr || scene == nullptr) {
        return;
    }
    g_SceneMapping.insert_or_assign(key, scene);
}


Scene* GetScene(const char* key) {
    if (key == nullptr) {
        return nullptr;
    }

    return g_SceneMapping.at(key);
}