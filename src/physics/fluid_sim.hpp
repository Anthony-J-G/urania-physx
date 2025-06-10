#pragma once

#include <cstdint>
#include <stdint.h>
#include <assert.h>


enum FieldType : uint8_t {
    FieldType__U = 0,
    FieldType__V,
    FieldType__S,
};

/*!
Simulation of a fluid using an Eulerian grid-based approach.
*/
class EulerFluid {
public:
    //! Fluid constructor
    EulerFluid(float density, int x, int y, int h);
    //! Explicit Fluid destructor to ensure proper freeing of particle buffers
    ~EulerFluid();
    //! Deleted copy construtor
    EulerFluid(const EulerFluid& other) = delete;
    //! Deleted copy assignment construtor
    EulerFluid& operator=(const EulerFluid& other) = delete;


public:
    //! Compute a single step of the fluid simulation
    void Simulate(const float deltatime, const float gravity, const uint8_t total_iterations, const float over_relax);

    int GetXCellCount() { return num_x; }
    int GetYCellCount() { return num_y; }
    int GetCellCount() { return cell_count; }
    float GetHeight() { return h; }
    float GetCellPressure(uint8_t index) { assert(index < cell_count); return p[index]; }
    float GetCellSmokeS(uint8_t index) { assert(index < cell_count); return s[index]; }
    float GetCellSmokeM(uint8_t index) { assert(index < cell_count); return m[index]; }

private:
    void Integrate(const float deltatime, const float gravity);
    void SolveIncompressibility(const float deltatime, const float over_relax);
    void Extrapolate();
    void AdvectVelocity(const float deltatime);    
    void AdvectSmoke(const float deltatime);

    float SampleField(int x, int y, FieldType field);
    float AverageU(int i, int j);
    float AverageV(int i, int j);

private:
    float density;
    int num_x;
    int num_y;
    int cell_count;
    float h;
    float* u;
    float* v;
    float* new_u;
    float* new_v;
    float* p;
    float* s;
    float* m;
    float* new_m;
};



// 
class FluidSimulationScene {
public:
    FluidSimulationScene();
    ~FluidSimulationScene() {};
    FluidSimulationScene(const FluidSimulationScene& other) = delete;
    FluidSimulationScene& operator=(const FluidSimulationScene& other) = delete;

    void Init();
    void Update(float deltatime);
    void Render();

private:
    EulerFluid* fluid;

    float simulation_height = 1.1;
    float cell_scale        = 1.0;
    float simulation_width  = 1.0;

    bool is_paused          = true;
    int frame_index         = 0;

    bool draw_smoke         = false;
    bool draw_grid          = true;
    bool draw_obstacle      = true;
	bool draw_streamlines   = false;
	bool draw_velocities    = false;
	bool draw_pressure      = true;

    int total_iterations    = 100;
	
    float over_relaxation   = 1.9;	
    float gravity           = 0.0;
};
