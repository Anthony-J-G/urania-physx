#include "scene_list_window.hpp"


// ** `raylib` Includes
#include <raylib.h>
#include <raymath.h>

// ** `Dear ImGui` Includes
#include <imgui.h>


// Scene List Window
//----------------------------------------------------------------------------------
void SceneListWindow::Setup(Editor* editor_ref) {
	is_open = true;
}


void SceneListWindow::Shutdown() {
}


void SceneListWindow::Update() {
}


void SceneListWindow::Draw() {
	if (!is_open) { return; }

	ImGui::Begin("Scene List");
	ImGui::End();
}
//---------------------------------------------------------------------------------- End Scene List Window

