#include "scene_list_window.hpp"

// ** `examples` includes
#include "editor.hpp"
#include "examples/scenes/scene.hpp"

// ** `raylib` Includes
#include <raylib.h>
#include <raymath.h>

// ** `Dear ImGui` Includes
#include <imgui.h>



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

	ImGuiTreeNodeFlags leafNodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
	leafNodeFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

	ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

	int categoryIndex = 0;
	const char* category = g_sceneEntries[categoryIndex].category;
	int i = 0;	

	ImGui::Begin(title);
	while (i < g_sceneCount) {
		bool categorySelected = strcmp(category, g_sceneEntries[parent->GetSceneSelection()].category) == 0;
		ImGuiTreeNodeFlags nodeSelectionFlags = categorySelected ? ImGuiTreeNodeFlags_Selected : 0;
		bool nodeOpen = ImGui::TreeNodeEx(category, nodeFlags | nodeSelectionFlags);

		if (nodeOpen) {
			while (i < g_sceneCount && strcmp(category, g_sceneEntries[i].category) == 0) {
				ImGuiTreeNodeFlags selectionFlags = 0;
				// if (s_settings.sampleIndex == i) {
				// 	selectionFlags = ImGuiTreeNodeFlags_Selected;
				// }
				ImGui::TreeNodeEx((void*)(intptr_t)i, leafNodeFlags | selectionFlags, "%s", g_sceneEntries[i].name);
				if (ImGui::IsItemClicked()) {
					parent->SetSceneSelection(i);
				}
				++i;
			}
			ImGui::TreePop();
		} else {
			while (i < g_sceneCount && strcmp(category, g_sceneEntries[i].category) == 0) {
				++i;
			}
		}

		if (i < g_sceneCount) {
			category = g_sceneEntries[i].category;
			categoryIndex = i;
		}
	}	
	
	ImGui::End();
}
//---------------------------------------------------------------------------------- End Scene List Window

