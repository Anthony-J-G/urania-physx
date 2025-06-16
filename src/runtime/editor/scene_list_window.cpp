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

	const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK", "LLLLLLL", "MMMM", "OOOOOOO" };

	ImGui::Begin("Scene List");	
	static int item_selected_idx = 0; // Here we store our selected data as an index.

	if (ImGui::BeginListBox("listbox 1")) {
		for (int n = 0; n < IM_ARRAYSIZE(items); n++) {
			const bool is_selected = (item_selected_idx == n);
			if (ImGui::Selectable(items[n], is_selected))
				item_selected_idx = n;

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
	}
    ImGui::EndListBox();

	if (ImGui::Button("Load")) {
		printf("Attempted to load scene: %s\n", items[item_selected_idx]);
	}
	
	ImGui::End();
}
//---------------------------------------------------------------------------------- End Scene List Window

