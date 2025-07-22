#include "backend.h"

// ** `raylib` Includes
#include <raylib.h>

// ** imgui Includes
#include "imgui.h"
#include "imgui_impl_raylib.h"

// ** `runtime` Includes
#include "editor/editor.hpp" // ImageViewerWindow, SceneViewWindow

// ** stdlib Includes
#include <math.h>

// ** `engine` Includes
#include <engine/scene.hpp>


Editor editor;


void InitBackend() {
    // ** Dear ImGui
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigWindowsMoveFromTitleBarOnly = true;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	// ** Editor
	editor.Initialize();
}


void DrawBackend() {
    ImGui_ImplRaylib_NewFrame();
    ImGui::NewFrame();
    ImGui_ImplRaylib_ProcessEvents();
    ImGui::DockSpaceOverViewport(ImGui::GetWindowDockID(), ImGui::GetMainViewport(), 0);

    editor.Draw();
    
    ImGui::Render();
    ImGui_ImplRaylib_RenderDrawData(ImGui::GetDrawData());
}


void UpdateBackend() {
    editor.Update();
}


bool ShouldQuit() {
    return !WindowShouldClose() && !editor.ShouldQuit();
}