#pragma once

#include <raylib.h>

#include <stdlib.h>


// Forward Declartions
class Editor;
class EditorWindow;
class ImageViewerWindow;
class SceneViewWindow;


class Editor {
public:
	Editor();	
	//! Explicit Fluid destructor to ensure proper freeing of particle buffers
    	~Editor();
    	//! Deleted copy construtor
    	Editor(const Editor& other) = delete;
    	//! Deleted copy assignment construtor
	Editor& operator=(const Editor& other) = delete;
	
	void Initialize();
	void Shutdown();

	void Update();
	void Draw();

private:
	void DrawMainMenuBar();

private:
	bool should_quit = false;
	bool show_imgui_demo = false;

	EditorWindow* editor_windows[];
	static const size_t window_count = 2;
};


class EditorWindow {

public:	
	virtual void Setup() = 0;
	virtual void Shutdown() = 0;
	virtual void Draw() = 0;
	virtual void Update() = 0;

	bool IsOpen() { return is_open; }
protected:
	bool Focused = false;
	Rectangle ContentRect = { 0 };
	bool is_open = false;
	RenderTexture ViewTexture;

};


class ImageViewerWindow : public EditorWindow {

public:
	void Setup() override;
	void Draw() override;
	void Update() override;
	void Shutdown() override;

	Texture ImageTexture;
	Camera2D Camera = { 0 };

	Vector2 LastMousePos = { 0 };
	Vector2 LastTarget = { 0 };
	bool Dragging = false;

	bool DirtyScene = false;

	enum class ToolMode
	{
		None,
		Move,
	};

	ToolMode CurrentToolMode = ToolMode::None;

	void UpdateRenderTexture();	
};


class SceneViewWindow : public EditorWindow {

public:

	void Setup() override;
	void Shutdown() override;
	void Show() override;
	void Update() override;	

	Camera3D Camera = { 0 };
	Texture2D GridTexture = { 0 };
};


class SceneListWindow : public EditorWindow {
};
