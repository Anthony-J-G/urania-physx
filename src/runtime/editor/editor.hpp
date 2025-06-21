#pragma once

#include "../common_dynamic_api.hpp"

#include <cstddef>
#include <raylib.h>

#include <stdlib.h>

#include <vector>

#include <physics/scene.hpp>



// Forward Declartions
class Editor;
class EditorWindow;


/*!
 *	Runtime specific class that is designed to be used for viewing scenes in an interactive development environment.
 */
class Editor final {

public:
	Editor();
	~Editor();
	Editor(const Editor& other) = delete;
	Editor& operator=(const Editor& other) = delete;
	
	void Initialize();
	void Shutdown();

	void Update();
	void Draw();

	bool ShouldQuit() { return should_quit; }
	const PhysicsLibrary& CallEngine();

	void SetCurrentScene(Scene_API* new_scene) { 
		if (new_scene == nullptr) {
			return;
		}
		if (current_scene != nullptr && current_scene->shutdown_fn != nullptr) {
			current_scene->shutdown_fn();
		}
		
		current_scene = new_scene;
		if (current_scene->initialize_fn) {
			current_scene->initialize_fn();
		}		
	} 
	Scene_API* GetCurrentScene() { return current_scene; }

private:
	void DrawMainMenuBar();

private:
	PhysicsLibrary engine;
	Scene_API* current_scene			= nullptr;

	bool should_quit 					= false;
	bool show_imgui_demo 				= false;

	std::vector<EditorWindow *> editor_windows;
	static const size_t window_count 	= 3;
};


class EditorWindow {

public:
	EditorWindow(const char* _title) : title(_title) {};
	virtual ~EditorWindow() {};
	virtual void Setup(Editor* _parent) = 0;
	virtual void Shutdown() = 0;
	virtual void Draw() = 0;
	virtual void Update() = 0;

	bool IsOpen() { return is_open; }
	void ToggleOpen() { is_open = !is_open; }

protected:	
	Rectangle ContentRect = { 0 };
	RenderTexture ViewTexture;
	
	const char* title;
	bool is_focused = false;
	bool is_open = false;

	Editor* parent;
};


// Utility Functions

float 	ScaleToDPI(float value);
int 	ScaleToDPI(int value); 