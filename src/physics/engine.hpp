#pragma once

#include "scene.hpp"


void InitEngine();
void RegisterScene(const char* key, Scene* scene);
extern "C" Scene* GetScene(const char* key);