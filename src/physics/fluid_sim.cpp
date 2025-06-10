#include "fluid_sim.hpp"


#include <cstddef>
#include <cstring>
#include <stdlib.h>
#include <raylib.h>
#include <rlgl.h>
#include <math.h>



// Function Declarations
Color GetScissorColor(float val, float minVal, float maxVal);


struct Obstacle {
    float radius        = 0.15;
    Vector2 position    = {0.0, 0.0};
    Color color         = {0, 0, 0};
};
static auto g_Obstacle = Obstacle{};
static auto g_Camera = Camera2D{
    .offset = { 0.0, 0.0},    // Camera offset (displacement from target)
    .target = {0, 0},         // Camera target (rotation and zoom origin)
    .rotation = 0,                  // Camera rotation in degrees
    .zoom = 1.0f,                   // Camera zoom (scaling), should be 1.0f by default
};
static Texture2D g_Texture;
static Image     g_Image;

// Fluid Simulation
//----------------------------------------------------------------------------------
Fluid::Fluid(float _density, int _x, int _y, int _h) {
    density     = _density;
    num_x       = _x + 2;
    num_y       = _y + 2;
    cell_count  = num_x * num_y;
    h           = _h;
    u           = static_cast<float*>(malloc(sizeof(float) * cell_count));
    v           = static_cast<float*>(malloc(sizeof(float) * cell_count));
    new_u       = static_cast<float*>(malloc(sizeof(float) * cell_count));
    new_v       = static_cast<float*>(malloc(sizeof(float) * cell_count));
    p           = static_cast<float*>(malloc(sizeof(float) * cell_count));
    s           = static_cast<float*>(malloc(sizeof(float) * cell_count));
    m           = static_cast<float*>(malloc(sizeof(float) * cell_count));
    new_m       = static_cast<float*>(malloc(sizeof(float) * cell_count));

    for (int i = 0; i < cell_count; i++) { m[i] = 1.0; }
}

Fluid::~Fluid() {
    free(static_cast<void*>(u));
    free(static_cast<void*>(v));
    free(static_cast<void*>(new_u));
    free(static_cast<void*>(new_v));
    free(static_cast<void*>(p));
    free(static_cast<void*>(s));
    free(static_cast<void*>(m));
    free(static_cast<void*>(new_m));
}


void Fluid::Integrate(const float deltatime, const float gravity) {
    int n = num_y;
    for (int i = 1; i < num_x; i++) {
        for (int j = 1; j < num_y; j++) {
            if (s[i*n + j] != 0.0 && s[i*n + j-1] != 0.0) {
                v[i*n + j] += gravity * deltatime;
            }
        }
    }
}


void Fluid::SolveIncompressibility(const float deltatime, const float over_relax) {
    int n = num_y;
    float cp = density * h / deltatime;

    for (int i = 1; i < num_x - 1; i++) {
        for (int j = 1; j < num_y - 1; j++) {
            if (s[i*n + j] == 0.0) {
                continue;
            }
            float s_mat = s[i*n + j];
            float s_x0 = s[(i-1)*n + j];
            float s_x1 = s[(i+1)*n + j];
            float s_y0 = s[i*n + j-1];
            float s_y1 = s[i*n + j+1];
            float s_total = s_x0 + s_x1 + s_y0 + s_y1;
            if (s_total == 0.0) {
                continue;
            }
            float div = (
                u[(i+1)*n + j] - u[i*n + j] + 
                v[i*n + j+1] - v[i*n + j]
            );

            float p_current = -div / s_total;
            p_current *= over_relax;
            p[i*n + j] += cp * p_current;

            u[i*n + j] -= s_x0 * p_current;
            u[(i+1)*n + j] += s_x1 * p_current;
            v[i*n + j] -= s_y0 * p_current;
            v[i*n + j+1] += s_y1 * p_current;
        }
    }
}


void Fluid::Extrapolate() {
    int n = num_y;
    for (size_t i = 0; i < num_x; i++) {
        u[i*n + 0] = u[i*n + 1];
        u[i*n + num_y-1] = u[i*n + num_y-2]; 
    }
    for (size_t j = 0; j < num_y; j++) {
        v[0*n + j] = v[1*n + j];
        v[(num_x-1)*n + j] = v[(num_x-2)*n + j];
    }
}


void Fluid::AdvectVelocity(const float deltatime) {
    memcpy(new_u, u, sizeof(float) * cell_count);
    memcpy(new_v, v, sizeof(float) * cell_count);

    float n = num_y;
	float h2 = 0.5 * h;

    for (size_t i = 1; i < num_x; i++) {
        for (size_t j = 1; j < num_y; j++) {

            // u component
            if (s[i*n + j] != 0.0 && s[(i-1)*n + j] != 0.0 && j < num_y - 1) {
                int x = i*h;
                int y = j*h + h2;
                float u_current = u[i*n + j];
                int v = AverageV(i, j);
//				var v = this.sampleField(x,y, V_FIELD);
                x = x - dt*u;
                y = y - dt*v;
                u = SampleField(x, y, FieldType__U);
                this.newU[i*n + j] = u;
            }
            // v component
            if (s[i*n + j] != 0.0 && s[i*n + j-1] != 0.0 && i < num_x - 1) {
                var x = i*h + h2;
                var y = j*h;
                var u = this.avgU(i, j);
//						var u = this.sampleField(x,y, U_FIELD);
                var v = this.v[i*n + j];
                x = x - dt*u;
                y = y - dt*v;
                v = this.sampleField(x,y, V_FIELD);
                this.newV[i*n + j] = v;
            }
        }	 
    }

    this.u.set(this.newU);
    this.v.set(this.newV);
}


void Fluid::AdvectSmoke(const float deltatime) {
}


float SampleField(int x, int y, FieldType field) {
    return 0.0;
}


float AverageU(int i, int j) {
    return 0.0;
}


float AverageV(int i, int j) {
    return 0.0;
}


void Fluid::Simulate(const float deltatime, const float gravity, const uint8_t total_iterations, float over_relax) {
    Integrate(deltatime, gravity);

    for (int i = 0; i < cell_count; i++) { p[i] = 0.0; }
    for (int i = 0; i < total_iterations; i++) {
        SolveIncompressibility(deltatime, over_relax);
    }

    Extrapolate();
    AdvectVelocity(deltatime);
    AdvectSmoke(deltatime);
    
}
//---------------------------------------------------------------------------------- End Fluid Simulation



// Fluid Simulation Scene
//----------------------------------------------------------------------------------
FluidSimulationScene::FluidSimulationScene() {
    fluid = new Fluid(1, 1, 1, 1);
}


void FluidSimulationScene::Init() {
    simulation_height = 1.1;
    cell_scale        = GetScreenHeight() / simulation_height;
    simulation_width  = GetScreenWidth() / cell_scale;

    g_Image = GenImageColor(GetScreenWidth(), GetScreenHeight(), GREEN);
    g_Texture = LoadTextureFromImage(g_Image);
}


void FluidSimulationScene::Update(float deltatime) {
    fluid->Simulate(deltatime, -9.81, 100, over_relaxation);
    frame_index += 1;
}


void FluidSimulationScene::Render() {    
    // TODO(anthony-j-g): actually render the fluid simulation to the screen.
    // BeginMode2D(g_Camera);
    
    // Draw the 3d grid, rotated 90 degrees and centered around 0,0 
    // just so we have something in the XY plane
    rlPushMatrix();
        rlTranslatef(0, 25*50, 0);
        rlRotatef(90, 1, 0, 0);
        DrawGrid(100, 100);
    rlPopMatrix();

    int n = fluid->GetYCellCount();

    float minimum_pressure = fluid->GetCellPressure(0);
    float maximum_pressure = fluid->GetCellPressure(0);

    for (size_t i = 0; i < fluid->GetCellCount(); i++) {
        minimum_pressure = fmin(minimum_pressure, fluid->GetCellPressure(i));
        maximum_pressure = fmax(maximum_pressure, fluid->GetCellPressure(i));
    }

    //id = c.getImageData(0,0, canvas.width, canvas.height)

    auto color = Color{255, 255, 255, 255};

    for (size_t i = 0; i < fluid->GetXCellCount(); i++) {
        for (size_t j = 0; j < fluid->GetYCellCount(); j++) {
            if (draw_pressure) {
                int index = i*n + j;
                float pressure = fluid->GetCellPressure(index);
                float s = fluid->GetCellSmokeM(index);
                color = GetScissorColor(pressure, minimum_pressure, maximum_pressure);
                if (draw_smoke) {
                    color.r = fmax(0.0, color.r - 255*s);
                    color.b = fmax(0.0, color.b - 255*s);
                    color.g = fmax(0.0, color.g - 255*s);
                }
            } else if (draw_smoke) {
                float s = fluid->GetCellSmokeM(i*n + j);
                color.r = 255*s;
                color.b = 255*s;
                color.g = 255*s;
    //            if (scene.sceneNr == 2)
    //                color = getSciColor(s, 0.0, 1.0);
            } else if (fluid->GetCellPressure(i*n + j) == 0.0) {
                color = Color{0, 0, 0, 0};
            }

            int x  = floor(i * fluid->GetHeight() * cell_scale);
            int y  = floor((j + 1) * fluid->GetHeight() * cell_scale);
            int cx = floor(cell_scale * 1.1 * fluid->GetHeight()) + 1;
            int cy = floor(cell_scale * 1.1 * fluid->GetHeight()) + 1;
                            
            for (int yi = y; yi < y + cy; yi++) { // For all the pixels in the cell, write the cell color
                int p = 4 * (yi * GetScreenWidth() + x);

                for (int xi = 0; xi < cx; xi++) {
                    auto pixel_loc = Vector2{
                        static_cast<float>(xi), static_cast<float>(yi)
                    };
                    ImageDrawPixelV(&g_Image, pixel_loc, color);
                }
            }
        }
    }

    UpdateTexture(g_Texture, g_Image.data);
    DrawTexture(g_Texture, GetScreenWidth() / 2 - g_Texture.width/2, GetScreenHeight() / 2 - g_Texture.height/2, WHITE);    

    if (draw_velocities) {
    //    c.strokeStyle = "#000000";	
    //    scale = 0.02;	

    //    for (var i = 0; i < f.numX; i++) {
    //        for (var j = 0; j < f.numY; j++) {

    //            var u = f.u[i*n + j];
    //            var v = f.v[i*n + j];

    //            c.beginPath();

    //            x0 = cX(i * h);
    //            x1 = cX(i * h + u * scale);
    //            y = cY((j + 0.5) * h );

    //            c.moveTo(x0, y);
    //            c.lineTo(x1, y);
    //            c.stroke();

    //            x = cX((i + 0.5) * h);
    //            y0 = cY(j * h );
    //            y1 = cY(j * h + v * scale)

    //            c.beginPath();
    //            c.moveTo(x, y0);
    //            c.lineTo(x, y1);
    //            c.stroke();

    //        }
    //    }
    }

    if (draw_streamlines) {
    //    var segLen = f.h * 0.2;
    //    var numSegs = 15;

    //    c.strokeStyle = "#000000";

    //    for (var i = 1; i < f.numX - 1; i += 5) {
    //        for (var j = 1; j < f.numY - 1; j += 5) {

    //            var x = (i + 0.5) * f.h;
    //            var y = (j + 0.5) * f.h;

    //            c.beginPath();
    //            c.moveTo(cX(x), cY(y));

    //            for (var n = 0; n < numSegs; n++) {
    //                var u = f.sampleField(x, y, U_FIELD);
    //                var v = f.sampleField(x, y, V_FIELD);
    //                l = Math.sqrt(u*u + v*v);
    //                // x += u/l * segLen;
    //                // y += v/l * segLen;
    //                x += u * 0.01;
    //                y += v * 0.01;
    //                if (x > f.numX * f.h)
    //                    break;

    //                c.lineTo(cX(x), cY(y));
    //            }
    //            c.stroke();
    //        }
    //    }
    }

    if (draw_obstacle) {
        int radius = g_Obstacle.radius + fluid->GetHeight();
        if (draw_pressure) {
            g_Obstacle.color = Color{0, 0,0};
        } else {
            g_Obstacle.color = Color{221,221, 221};
        }
        DrawCircleV(g_Obstacle.position, radius, g_Obstacle.color);    
    }

    if (draw_pressure) {
    //    var s = "pressure: " + minP.toFixed(0) + " - " + maxP.toFixed(0) + " N/m";
    //    c.fillStyle ="#000000";
    //    c.font = "16px Arial";
    //    c.fillText(s, 10, 35);
    }

    // EndMode2D();
}
//---------------------------------------------------------------------------------- End Fluid Simulation Scene


Color GetScissorColor(float val, float minVal, float maxVal) {
    float r = 0.0, g = 0.0, b = 0.0;
    val = fmin(fmax(val, minVal), maxVal- 0.0001);
    float d = maxVal - minVal;
    val = d == 0.0 ? 0.5 : (val - minVal) / d;
    float m = 0.25;
    unsigned char num = floor(val / m);
    float s = (val - num * m) / m;

    switch (num) {
        case 0 : r = 0.0; g = s; b = 1.0; break;
        case 1 : r = 0.0; g = 1.0; b = 1.0-s; break;
        case 2 : r = s; g = 1.0; b = 0.0; break;
        case 3 : r = 1.0; g = 1.0 - s; b = 0.0; break;
    }

    return Color{
        static_cast<unsigned char>(255*r),
        static_cast<unsigned char>(255*g),
        static_cast<unsigned char>(255*b),
        255,
    };
}