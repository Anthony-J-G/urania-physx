/*******************************************************************************************
*
*   raylib-extras [ImGui] example - editor
*
*	This is a more complex ImGui Integration
*	It shows how to build windows on top of 2d and 3d views using a render texture
*
*   Copyright (c) 2021 Jeffery Myers
*
********************************************************************************************/


#include "raylib.h"
#include "raymath.h"

#include "imgui.h"
#include "rlImGui.h"
#include "imgui_impl_raylib.h"
#include "rlImGuiColors.h"

#include <math.h>

#if defined(_WIN32) || defined(_WIN64)
#	include "win32_dynamic_api.h"
#endif // Win32 or Win64


bool Quit = false;

bool ImGuiDemoOpen = false;

// DPI scaling functions
float ScaleToDPIF(float value)
{
    return GetWindowScaleDPI().x * value;
}

int ScaleToDPII(int value)
{
    return int(GetWindowScaleDPI().x * value);
}


ImageViewerWindow ImageViewer;
SceneViewWindow SceneView;

void DoMainMenu()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
				Quit = true;

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Window"))
		{
			ImGui::MenuItem("ImGui Demo", nullptr, &ImGuiDemoOpen);
			ImGui::MenuItem("Image Viewer", nullptr, &ImageViewer.Open);
			ImGui::MenuItem("3D View", nullptr, &SceneView.Open);

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

int main(int argc, char* argv[])
{
	// Initialization
	//--------------------------------------------------------------------------------------
	int screenWidth  = 800;
	int screenHeight = 600;

	// do not set the FLAG_WINDOW_HIGHDPI flag, that scales a low res framebuffer up to the native resolution.
	// use the native resolution and scale your geometry.
	int windowFlags = FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_MAXIMIZED; 
	SetConfigFlags(windowFlags);
	InitWindow(screenWidth, screenHeight, "raylib-Extras [ImGui] example - Editor Example");
	SetTargetFPS(144);
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

	// FluidSimulationScene scene;
	// scene.Init();

	ImageViewer.Setup();
	ImageViewer.Open = true;

	SceneView.Setup();
	SceneView.Open = true;

	// Main game loop
	while (!WindowShouldClose() && !Quit) { // Detect window close button or ESC key
		// Update Step
        	//----------------------------------------------------------------------------------
		float deltatime = GetFrameTime();

		ImageViewer.Update();
		SceneView.Update();
		// scene.Update(deltatime);
		//----------------------------------------------------------------------------------

		// Render Step
		//----------------------------------------------------------------------------------
		BeginDrawing();
		ClearBackground(DARKGRAY);

		ImGui_ImplRaylib_NewFrame();
		ImGui::NewFrame();
		ImGui_ImplRaylib_ProcessEvents();
		ImGui::DockSpaceOverViewport(ImGui::GetWindowDockID(), ImGui::GetMainViewport(), 0);

		DoMainMenu();		

		if (ImGuiDemoOpen)
			ImGui::ShowDemoWindow(&ImGuiDemoOpen);

		if (ImageViewer.Open)
			ImageViewer.Show();

		if (SceneView.Open)
			SceneView.Show();
		
		// scene.Render();

		ImGui::Render();
    		ImGui_ImplRaylib_RenderDrawData(ImGui::GetDrawData());

		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------   
	ImageViewer.Shutdown();
	SceneView.Shutdown();

	ImGui_ImplRaylib_Shutdown();
	ImGui::DestroyContext();
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}
