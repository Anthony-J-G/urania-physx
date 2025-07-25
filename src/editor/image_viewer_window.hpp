#pragma once

#include <raylib.h>

#include "editor.hpp"


class ImageViewerWindow : public EditorWindow {

public:
	ImageViewerWindow(const char* _title);
	void Setup(Editor* _parent) override;
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