#pragma once

#include <raylib.h>

// Forward Declartions
class Editor;
class EditorWindow;
class ImageViewerWindow;
class SceneViewWindow;


class Editor {
public:
	Editor();
	
	void Update();
	void Show();

private:
	void CreateMainMenuBar();

private:
	bool should_quit = false;
	bool show_imgui_demo = false;

	EditorWindow* editor_windows[];
};




void DoMainMenu();

class EditorWindow {

public:
	bool Open = false;

	RenderTexture ViewTexture;

	virtual void Setup() = 0;
	virtual void Shutdown() = 0;
	virtual void Show() = 0;
	virtual void Update() = 0;

	bool Focused = false;

	Rectangle ContentRect = { 0 };
};


class ImageViewerWindow : public EditorWindow {

public:
	void Setup() override;
	void Show() override;
	void Update() override;

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
	void Shutdown() override;
	
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