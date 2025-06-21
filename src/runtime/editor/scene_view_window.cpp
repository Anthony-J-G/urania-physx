#include "scene_view_window.hpp"


// ** `raylib` Includes
#include <raylib.h>
#include <raymath.h>

// ** `Dear ImGui` Includes
#include <imgui.h>

#include "editor.hpp"
#include "rlImGui.h"


// Scene View Window
//----------------------------------------------------------------------------------
SceneViewWindow::SceneViewWindow(const char* _title): EditorWindow(_title) {
}


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

	if (is_focused && parent->GetCurrentScene() != nullptr) {
		parent->GetCurrentScene()->update_fn(GetFrameTime());
	}

	BeginTextureMode(ViewTexture);
	ClearBackground(SKYBLUE);
	
	if (parent->GetCurrentScene() != nullptr) {
		parent->GetCurrentScene()->render_fn();
	}

	EndTextureMode();
}


void SceneViewWindow::Draw() {
	if (!is_open) { return; }
		
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::SetNextWindowSizeConstraints(ImVec2(ScaleToDPI(400.0f), ScaleToDPI(400.0f)), ImVec2((float)GetScreenWidth(), (float)GetScreenHeight()));

	if (ImGui::Begin(title, &is_open, ImGuiWindowFlags_NoScrollbar)) {
		is_focused = ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows);
		// draw the view
		rlImGuiImageRenderTextureFit(&ViewTexture, true);
	}
	ImGui::End();
	ImGui::PopStyleVar();
}
//---------------------------------------------------------------------------------- End Scene View Window