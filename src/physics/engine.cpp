#include "engine.hpp"
#include "scene.hpp"

#include "scenes/sample.hpp"


void InitEngine() {
    RegisterScene(GenerateSampleScene);
}