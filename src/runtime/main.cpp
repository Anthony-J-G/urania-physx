// ** `raylib` Includes
#include "raylib.h"

// ** imgui Includes
#include "imgui.h"
#include "imgui_impl_raylib.h"

// ** `runtime` Includes
#include "editor.hpp" // ImageViewerWindow, SceneViewWindow

// ** stdlib Includes
#include <math.h>

// ** `physics` Includes
#include <physics/scene.hpp>


int main(int argc, char* argv[]) {
	// Initialization
	//--------------------------------------------------------------------------------------
	int screenWidth  = 800;
	int screenHeight = 600;

	Editor editor;	

	// do not set the FLAG_WINDOW_HIGHDPI flag, that scales a low res framebuffer up to the native resolution.
	// use the native resolution and scale your geometry.
	int windowFlags = FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_MAXIMIZED | FLAG_WINDOW_RESIZABLE;
	SetConfigFlags(windowFlags);
	InitWindow(screenWidth, screenHeight, "raylib-Extras [ImGui] example - Editor Example");	
	SetTargetFPS(144);
	SetExitKey(KEY_NULL);
	// Set Window Resolution to something Sensible
	int currentMonitor = GetCurrentMonitor();
	float aspectRatio = static_cast<float>(GetMonitorHeight(currentMonitor)) / GetMonitorWidth(currentMonitor);
	screenWidth  = static_cast<int>(GetMonitorWidth(currentMonitor) * 0.90f);
	screenHeight = static_cast<int>(screenWidth * aspectRatio);
	SetWindowSize(screenWidth, screenHeight);
	SetWindowPosition(
		abs(screenWidth - GetMonitorWidth(currentMonitor)) / 2,
		abs(screenHeight - GetMonitorHeight(currentMonitor)) / 2
	);
	// windowFlags ^= FLAG_WINDOW_HIDDEN;
	// SetWindowState(windowFlags);

	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigWindowsMoveFromTitleBarOnly = true;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	editor.Initialize();

	// Main game loop
	while (!WindowShouldClose() && !editor.ShouldQuit()) { // Detect window close button or ESC key
		// Update Step
        	//----------------------------------------------------------------------------------
		float deltatime = GetFrameTime();
		editor.Update();
		// current_scene.Update(deltatime);
		//----------------------------------------------------------------------------------

		// Render Step
		//----------------------------------------------------------------------------------
		BeginDrawing();
		ClearBackground(DARKGRAY);

		ImGui_ImplRaylib_NewFrame();
		ImGui::NewFrame();
		ImGui_ImplRaylib_ProcessEvents();
		ImGui::DockSpaceOverViewport(ImGui::GetWindowDockID(), ImGui::GetMainViewport(), 0);

		editor.Draw();
		
		ImGui::Render();
    	ImGui_ImplRaylib_RenderDrawData(ImGui::GetDrawData());

		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------   
	editor.Shutdown();
	// current_scene.shutdown();

	ImGui_ImplRaylib_Shutdown();
	ImGui::DestroyContext();
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}
