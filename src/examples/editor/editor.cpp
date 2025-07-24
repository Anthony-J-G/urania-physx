#include "editor.hpp"

// ** `stdlib` includes
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>

// ** `raylib` Includes
#include <raylib.h>
#include <raymath.h>

// ** `Dear ImGui` Includes
#include <imgui.h>
#include <imgui_impl_raylib.h>

// ** Editor Windows
#include "control_window.hpp"
#include "scene_view_window.hpp"
#include "scene_list_window.hpp"



// Editor Window Enumeration
//----------------------------------------------------------------------------------
enum EditorWindowIndex {
	EDITOR_IMAGE_VIEWER = 0,
	EDITOR_SCENE_VIEWER = 1,
	EDITOR_SCENE_LIST	= 2,
	EDITOR_COMPILER		= 3,
};
//---------------------------------------------------------------------------------- End Utility Functions

// Utility Functions
//----------------------------------------------------------------------------------
float ScaleToDPI(float value) {
    return GetWindowScaleDPI().x * value;
}


int ScaleToDPI(int value) {
    return int(GetWindowScaleDPI().x * value);
}
//---------------------------------------------------------------------------------- End Utility Functions


// Editor
//----------------------------------------------------------------------------------
Editor::Editor() {	
}


void Editor::Update() {
	for (size_t i = 0; i < window_count; i++) {
		editor_windows[i]->Update();
	}
}


void Editor::Draw() {
	DrawMainMenuBar();

	for (size_t i = 0; i < window_count; i++) {		
		editor_windows[i]->Draw();
	}

	if (show_imgui_demo) {
		ImGui::ShowDemoWindow(&show_imgui_demo);
	}	
}


void Editor::Initialize() {
    EditorWindow* windows[] = {
        dynamic_cast<EditorWindow *>(new SceneViewWindow("Viewport")),
		dynamic_cast<EditorWindow *>(new SceneListWindow("Scene List")),
		dynamic_cast<EditorWindow *>(new ControlsWindow("Controls")),
    };
	window_count = sizeof(windows) / sizeof(EditorWindow *);
	
	editor_windows = static_cast<EditorWindow **>(malloc(sizeof(windows)));
	memset(editor_windows, 0, window_count);

	for (size_t i = 0; i < window_count; i++) {
		editor_windows[i] = windows[i];
		if (editor_windows[i] == nullptr) {
			Shutdown();
			printf("panic");
			exit(1);
		}

	    editor_windows[i]->Setup(this);
	}

	ImGui_ImplRaylib_Init();
}


void Editor::Shutdown() {
	for (size_t i = 0; i < window_count; i++) {
		delete editor_windows[i];
	}
	free(editor_windows);
}


Editor::~Editor() {
	// Ensure resources are cleaned up on destruction
	Shutdown();
}


bool Editor::ShouldQuit() {
	return should_quit || WindowShouldClose();
}


void Editor::DrawMainMenuBar() {
	static bool should_autosave = false;

	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("New Project...", NULL, false, false)) {
			}
			if (ImGui::MenuItem("Open Project...", NULL, false, false)) {
			}
			if (ImGui::MenuItem("Save Project", NULL, false, false)) {
			}
			ImGui::Separator();
			ImGui::MenuItem("Toggle Autosave", NULL, &should_autosave, true);
			ImGui::Separator();
			if (ImGui::MenuItem("Export", NULL, false, false)) {
			}
			if (ImGui::MenuItem("Project Settings", NULL, false, false)) {
			}
			if (ImGui::MenuItem("Quit to Project List", NULL, false, false)) {
			}
			if (ImGui::MenuItem("Quit", NULL, false, false)) {
				should_quit = true;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit")) {
			if (ImGui::MenuItem("Undo", "Ctrl+Z", false, true)) {
				printf("Tried to undo!\n");
			}
			if (ImGui::MenuItem("Redo", NULL, false, false)) {
			}
			if (ImGui::MenuItem("Undo Last Global Action", NULL, false, false)) {
			}
			if (ImGui::MenuItem("Undo Last Global Action", NULL, false, false)) {
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Copy", NULL, false, true)) {
			}
			if (ImGui::MenuItem("Cut", NULL, false, false)) {
			}
			if (ImGui::MenuItem("Paste", NULL, false, false)) {
			}
			if (ImGui::MenuItem("Show Clipboard History", NULL, false, false)) {
			}
			if (ImGui::MenuItem("duplicate", NULL, false, false)) {
			}			
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View")) {
			ImGui::MenuItem("ImGui Demo", nullptr, &show_imgui_demo);
			ImGui::Separator();
            for (size_t i = 0; i < window_count; i++) {
				auto window = editor_windows[i];
            	ImGui::MenuItem(window->title, nullptr, &window->is_open);
            }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Scene")) {
			if (ImGui::MenuItem("Exit")) {
				should_quit = true;
			}				
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Editor")) {
			if (ImGui::MenuItem("Exit")) {
				should_quit = true;
			}				
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Build")) {
			if (ImGui::MenuItem("Exit")) {
				should_quit = true;
			}				
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}
//----------------------------------------------------------------------------------