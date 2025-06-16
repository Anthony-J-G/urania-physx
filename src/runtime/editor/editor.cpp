#include "editor.hpp"

// ** `raylib` Includes
#include <raylib.h>
#include <raymath.h>

// ** `Dear ImGui` Includes
#include <imgui.h>

// ** Editor Windows
#include "scene_list_window.hpp"
#include "scene_view_window.hpp"
#include "image_viewer_window.hpp"

// ** `win32` Includes
#if defined(_WIN32) || defined(_WIN64)
#	include "../win32_dynamic_api.hpp"
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

	auto image_viewer_window	= new ImageViewerWindow();
	auto scene_view_window		= new SceneViewWindow();
	auto scene_list_window		= new SceneListWindow();

	editor_windows.reserve(3);
	editor_windows.push_back(static_cast<EditorWindow *>(image_viewer_window));
	editor_windows.push_back(static_cast<EditorWindow *>(scene_view_window));
	editor_windows.push_back(static_cast<EditorWindow *>(scene_list_window));

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


const PhysicsLibrary& Editor::CallEngine() {
	return engine;
}


void Editor::DrawMainMenuBar() {
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Exit")) {
				should_quit = true;
			}				
			ImGui::EndMenu();
		}		

		if (ImGui::BeginMenu("Window")) {
			ImGui::MenuItem("ImGui Demo", nullptr, &show_imgui_demo);
			// TODO(anthony-j-g): Fix these so they properly reference the window's boolean
			// ImGui::MenuItem("Image Viewer", nullptr, &editor_windows[EDITOR_IMAGE_VIEWER]);
			// ImGui::MenuItem("3D View", nullptr, &editor_windows[EDITOR_SCENE_VIEWER]);

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}
//----------------------------------------------------------------------------------