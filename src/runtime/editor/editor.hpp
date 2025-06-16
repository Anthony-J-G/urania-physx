#pragma once

#include "../common_dynamic_api.hpp"

#include <raylib.h>

#include <stdlib.h>

#include <vector>

#include <physics/scene.hpp>



// Forward Declartions
class Editor;
class EditorWindow;
class ImageViewerWindow;
class SceneViewWindow;



/*!
 *	Runtime specific class that is designed to be used for viewing scenes in an interactive development environment.
 */
class Editor final {
	friend SceneViewWindow;
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

private:
	void DrawMainMenuBar();

private:
	PhysicsLibrary physics_api;
	Scene* current_scene				= nullptr;

	bool should_quit 					= false;
	bool show_imgui_demo 				= false;

	std::vector<EditorWindow *> editor_windows;
	static const size_t window_count 	= 3;
};


class EditorWindow {

public:	
	virtual ~EditorWindow() {};
	virtual void Setup(Editor* editor_ref) = 0;
	virtual void Shutdown() = 0;
	virtual void Draw() = 0;
	virtual void Update() = 0;

	bool IsOpen() { return is_open; }
	void ToggleOpen() { is_open = !is_open; }

protected:	
	Rectangle ContentRect = { 0 };
	RenderTexture ViewTexture;
	
	bool is_focused = false;
	bool is_open = false;

	Editor* parent;
};


class ImageViewerWindow : public EditorWindow {

public:
	void Setup(Editor* editor_ref) override;
	void Shutdown() override;
	void Draw() override;
	void Update() override;

	Texture ImageTexture;
	Camera2D Camera = { 0 };

	Vector2 LastMousePos = { 0 };
	Vector2 LastTarget = { 0 };
	bool Dragging = false;

	bool DirtyScene = false;

	enum class ToolMode {
		None,
		Move,
	};

	ToolMode CurrentToolMode = ToolMode::None;

	void UpdateRenderTexture();	
};


class SceneViewWindow : public EditorWindow {
	friend Editor;
public:
	void Setup(Editor* editor_ref) override;
	void Shutdown() override;
	void Update() override;
	void Draw() override;

	Camera3D Camera = { 0 };
	Texture2D GridTexture = { 0 };
};


class SceneListWindow : public EditorWindow {

public:
	void Setup(Editor* editor_ref) override;
	void Shutdown() override;
	void Update() override;
	void Draw() override;
};
