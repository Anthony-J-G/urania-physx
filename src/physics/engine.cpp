#include "engine.hpp"
#include "scene.hpp"

#include "scenes/sample.hpp"
#include "scenes/euler_fluid_sim_2d.hpp"


void InitEngine() {
    RegisterScene(GenerateSampleScene);
    RegisterScene(GenerateEulerFluidSim2D);
}