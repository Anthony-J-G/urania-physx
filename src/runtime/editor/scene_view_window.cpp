#include "scene_view_window.hpp"


// ** `raylib` Includes
#include <raylib.h>
#include <raymath.h>

// ** `Dear ImGui` Includes
#include <imgui.h>

#include "rlImGui.h"


// Scene View Window
//----------------------------------------------------------------------------------
void SceneViewWindow::Setup(Editor* editor_ref) {
	parent = editor_ref;
	// parent->CallEngine().GetScene("OscillatingCircle");
	ViewTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

	Camera.fovy = 45;
	Camera.up.y = 1;
	Camera.position.y = 3;
	Camera.position.z = -25;

	is_open = true;
	Image img = GenImageChecked(ScaleToDPI(256), ScaleToDPI(256), ScaleToDPI(32), ScaleToDPI(32), DARKGRAY, WHITE);
	GridTexture = LoadTextureFromImage(img);
	UnloadImage(img);
	GenTextureMipmaps(&GridTexture);
	SetTextureFilter(GridTexture, TEXTURE_FILTER_ANISOTROPIC_16X);
	SetTextureWrap(GridTexture, TEXTURE_WRAP_CLAMP);
}


void SceneViewWindow::Shutdown() {
	// parent->physics_api.Scene__Shutdown();
	UnloadRenderTexture(ViewTexture);
	UnloadTexture(GridTexture);
}


void SceneViewWindow::Update() {
	if (!is_open) { return; }

	if (IsWindowResized()) {
		UnloadRenderTexture(ViewTexture);
		ViewTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
	}	
	// parent->physics_api.Scene__Update(GetFrameTime());
	float period = 10;
	float magnitude = 25;

	Camera.position.x = sinf(float(GetTime() / period)) * magnitude;

	BeginTextureMode(ViewTexture);
	ClearBackground(SKYBLUE);

	// parent->physics_api.Scene__Render();
	// BeginMode3D(Camera);
// 
	// // grid of cube trees on a plane to make a "world"
	// DrawPlane(Vector3{ 0, 0, 0 }, Vector2{ 50, 50 }, BEIGE); // simple world plane
	// float spacing = 4;
	// int count = 5;
// 
	// for (float x = -count * spacing; x <= count * spacing; x += spacing) {
	// 	for (float z = -count * spacing; z <= count * spacing; z += spacing) {
	// 		Vector3 pos = { x, 0.5f, z };
// 
	// 		Vector3 min = { x - 0.5f,0,z - 0.5f };
	// 		Vector3 max = { x + 0.5f,1,z + 0.5f };
// 
	// 		DrawCube(Vector3{ x, 1.5f, z }, 1, 1, 1, GREEN);
	// 		DrawCube(Vector3{ x, 0.5f, z }, 0.25f, 1, 0.25f, BROWN);
	// 	}
	// }
// 
	// EndMode3D();
	EndTextureMode();
}


void SceneViewWindow::Draw() {
	if (!is_open) { return; }
		
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::SetNextWindowSizeConstraints(ImVec2(ScaleToDPI(400.0f), ScaleToDPI(400.0f)), ImVec2((float)GetScreenWidth(), (float)GetScreenHeight()));

	if (ImGui::Begin("3D View", &is_open, ImGuiWindowFlags_NoScrollbar)) {
		is_focused = ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows);
		// draw the view
		rlImGuiImageRenderTextureFit(&ViewTexture, true);
	}
	ImGui::End();
	ImGui::PopStyleVar();
}
//---------------------------------------------------------------------------------- End Scene View Window