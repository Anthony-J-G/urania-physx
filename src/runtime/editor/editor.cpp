#include "editor.hpp"

// ** `raylib` Includes
#include <cstddef>
#include <raylib.h>
#include <raymath.h>

// ** `Dear ImGui` Includes
#include <imgui.h>

// ** Editor Windows
#include "compiler_window.hpp"
#include "scene_list_window.hpp"
#include "scene_view_window.hpp"
#include "image_viewer_window.hpp"

// ** `win32` Includes
#if defined(_WIN32) || defined(_WIN64)
#	include "runtime/dynlib/win32_load.hpp"
	using namespace win32;
#else // Win32 or Win64
// TODO(anthony-j-g): enable this for Linux and OSX
#	error "Dynamic reloading not supported in OS other than Windows!"
#endif


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
	for (EditorWindow* window : editor_windows) {
		window->Update();
	}
}


void Editor::Draw() {
	DrawMainMenuBar();

	for (EditorWindow* window : editor_windows) {
		window->Draw();
	}

	if (show_imgui_demo) {
		ImGui::ShowDemoWindow(&show_imgui_demo);
	}	
}


void Editor::Initialize() {
	// Load Dynamic APIs
	LoadEngineLibrary(engine);

	auto image_viewer_window	= new ImageViewerWindow("Image Viewer");
	auto scene_view_window		= new SceneViewWindow("Viewport");
	auto scene_list_window		= new SceneListWindow("Scene List");
	auto compiler_window			= new CompilerWindow("Compiler");

	editor_windows.reserve(4);
	editor_windows.push_back(static_cast<EditorWindow *>(image_viewer_window));
	editor_windows.push_back(static_cast<EditorWindow *>(scene_view_window));
	editor_windows.push_back(static_cast<EditorWindow *>(scene_list_window));
	editor_windows.push_back(static_cast<EditorWindow *>(compiler_window));

	for (EditorWindow* window: editor_windows) {
		window->Setup(this);
	}
}


void Editor::Shutdown() {
	for (EditorWindow* window : editor_windows) {
		delete window;
	}
	editor_windows.clear();
}


Editor::~Editor() {
	// Ensure resources are cleaned up on destruction
	Shutdown();
}


const EngineLibrary& Editor::CallEngine() {
	return engine;
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
			ImGui::MenuItem("Image Viewer", nullptr, &static_cast<SceneViewWindow*>(editor_windows[EDITOR_IMAGE_VIEWER])->is_open);
			ImGui::MenuItem("Scene List", nullptr, &static_cast<SceneListWindow*>(editor_windows[EDITOR_SCENE_LIST])->is_open);
			ImGui::MenuItem("Viewport", nullptr, &static_cast<SceneViewWindow*>(editor_windows[EDITOR_SCENE_VIEWER])->is_open);
			ImGui::Separator();
			ImGui::MenuItem("Compiler", nullptr, &static_cast<CompilerWindow*>(editor_windows[EDITOR_COMPILER])->is_open);

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