#pragma once

#include "editor.hpp"

#include <raylib.h>


class SceneViewWindow : public EditorWindow {
	friend Editor;
public:
	SceneViewWindow(const char* _title);
	void Setup(Editor* editor_ref) override;
	void Shutdown() override;
	void Update() override;
	void Draw() override;

	Camera3D Camera = { 0 };
	Texture2D GridTexture = { 0 };
};