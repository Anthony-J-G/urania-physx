#pragma once

#include <raylib.h>

#include <stdlib.h>

#include "examples/scenes/scene.hpp"



// Forward Declartions
class Editor;
class EditorWindow;


/*!
* 	@brief
*	Informs the simulation about what the current running state should be via GUI interactions, as well as provide a platform
*	in which to render the simulation.
*/
class Editor final {

public:
	Editor();
	~Editor();
	Editor(const Editor& other) = delete;				// Deleted Copy Constructor
	Editor(const Editor&& other) = delete;	// Deleted Move Constructor
	Editor& operator=(const Editor& other) = delete;	// Deleted Copy Assignment Constructor
	
	/*!
	* 	@brief
	*	Populates the @p editor_windows member array with casted pointers of the child classes and
	*  	initializes Raylib ImGui implementation backend.
	*/	
	void Initialize();

	/*!
	* 	@brief
	*	Deletes all of the elements of the @p editor_windows member array and frees the heap
	*	allocated memory for the buffer.
	*/	
	void Shutdown();

	/*!
	* 	@brief
	*	Iterates over all of the elements of the @p editor_windows member array and calls their
	*	corresponding Update() function.
	*/
	void Update();

	/*!
	* 	@brief
	*	Iterates over all of the elements of the @p editor_windows member array and calls their
	*	corresponding Draw() function. Additionally shows the ImGui demo window if the option
	*	selected and renders the menu bars to the screen.
	*/
	void Draw();

	/*!
	* 	@brief
	*	Populates the @p editor_windows member array with casted pointers of the child classes and
	*  	initializes Raylib ImGui implementation backend.
	*/
	bool ShouldQuit();

	/*!
	* 	@brief
	*	Populates the @p editor_windows member array with casted pointers of the child classes and
	*  	initializes Raylib ImGui implementation backend.
	*/
	inline int GetSceneSelection() { return scene_selection; }

	/*!
	* 	@brief
	*	Populates the @p editor_windows member array with casted pointers of the child classes and
	*  	initializes Raylib ImGui implementation backend.
	*/
	inline void SetSceneSelection(int new_scene) {
		if (g_sceneCount <= new_scene || new_scene < 0) {
			return;
		}
		scene_selection = new_scene;
	}

private:
	/*!
	* 	@brief
	*	Populates the @p editor_windows member array with casted pointers of the child classes and
	*  	initializes Raylib ImGui implementation backend.
	*/
	void DrawMainMenuBar();

private:	
	int scene_selection					= 0;
	bool should_quit 					= false;
	bool show_imgui_demo 				= false;

	EditorWindow** editor_windows 		= nullptr;
	size_t window_count 				= 0;
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