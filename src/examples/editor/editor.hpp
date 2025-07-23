#pragma once

#include <raylib.h>

#include <stdlib.h>

#include "examples/scenes/scene.hpp"



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

	int GetSceneSelection() { return scene_selection; }
	void SetSceneSelection(int new_scene) {
		if (g_sceneCount <= new_scene || new_scene < 0) {
			return;
		}
		scene_selection = new_scene;
	}

private:
	void DrawMainMenuBar();

private:	
	int scene_selection					= 0;
	bool should_quit 					= false;
	bool show_imgui_demo 				= false;

	EditorWindow** editor_windows 		= nullptr;
	size_t window_count 				= 3;
};


class EditorWindow {
	friend Editor;
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
	const char* title;
	bool is_focused = false;
	bool is_open = false;

	Editor* parent;
};


// Utility Functions

float 	ScaleToDPI(float value);
int 	ScaleToDPI(int value); 