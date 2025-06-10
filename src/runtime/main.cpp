#include <stdlib.h>

#include <raylib.h>
#include <physics/fluid_sim.hpp>


int main(void) {

	InitWindow(1280, 720, "raylib - fluid simulator context");

	FluidSimulationScene scene;
	scene.Init();

	while (!WindowShouldClose()) {
		// Update Step
        //----------------------------------------------------------------------------------
		float deltatime = GetFrameTime();
		scene.Update(deltatime);
		//----------------------------------------------------------------------------------

		// Render Step
        //----------------------------------------------------------------------------------
		BeginDrawing(); {
			ClearBackground(RAYWHITE);
			scene.Render();
		}
		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
    //--------------------------------------------------------------------------------------
	CloseWindow(); // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return EXIT_SUCCESS;
}
