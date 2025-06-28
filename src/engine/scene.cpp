#include "scene.hpp"


// TODO(anthony-j-g): replace this with a more reasonable, non-STL data structure
#include <unordered_map>
#include <vector>

using Map = std::unordered_map<const char *, Scene_API>;

static Map g_SceneMapping;


bool RegisterScene(Scene_API(*fn)()) {
    auto scene = fn();    
    if (scene.name == nullptr) {
        return false;
    }
    auto result = g_SceneMapping.emplace(scene.name, scene);

    return result.second;
}


Scene_API* GetScene(const char* key) {
    if (key == nullptr) {
        return nullptr;
    }        
    return &g_SceneMapping.at(key);
}


std::vector<const char*> GetSceneNames() {
    std::vector<const char *> names;
    names.reserve(g_SceneMapping.size());

    for (const std::pair<const char*, Scene_API>& name : g_SceneMapping) {
        names.emplace_back(name.first);
    }

    return names;
}