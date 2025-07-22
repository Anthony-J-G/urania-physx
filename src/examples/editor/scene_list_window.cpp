#include "scene_list_window.hpp"
#include "editor.hpp"


// ** `raylib` Includes
#include <raylib.h>
#include <raymath.h>

// ** `Dear ImGui` Includes
#include <imgui.h>
#include <vector>


// Scene List Window
//----------------------------------------------------------------------------------
SceneListWindow::SceneListWindow(const char* _title): EditorWindow(_title) {
}


void SceneListWindow::Setup(Editor* editor_ref) {
	parent = editor_ref;
	is_open = true;
}


void SceneListWindow::Shutdown() {
}


void SceneListWindow::Update() {
}


void SceneListWindow::Draw() {
	if (!is_open) { return; }

	auto engine_ref = parent->CallEngine();
	std::vector<const char *> names = {};

	if (engine_ref) { names = engine_ref->GetSceneNames(); }
	const char** items = names.data();
	// const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK", "LLLLLLL", "MMMM", "OOOOOOO" };

	ImGui::Begin("Scene List");	
	static int item_selected_idx = 0; // Here we store our selected data as an index.

	if (ImGui::BeginListBox("listbox 1")) {
		for (int n = 0; n < names.size(); n++) {
		// for (int n = 0; n < IM_ARRAYSIZE(items); n++) {
			const bool is_selected = (item_selected_idx == n);
			if (ImGui::Selectable(items[n], is_selected))
				item_selected_idx = n;

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
	}
    ImGui::EndListBox();

	if (names.size() != 0 && ImGui::Button("Load")) {
		printf("INFO: [editor] Attempted to load scene: %s\n", items[item_selected_idx]);
		Scene_API* scene = nullptr;
		if (engine_ref) { scene = engine_ref->GetScene(items[item_selected_idx]); };
		if (scene != nullptr) {
			parent->SetCurrentScene(scene);
		}
	}
	
	ImGui::End();
}
//---------------------------------------------------------------------------------- End Scene List Window

