#pragma once

#include "editor.hpp"



class SceneListWindow : public EditorWindow {
	friend Editor;
public:
	SceneListWindow(const char* _title);
	void Setup(Editor* _parent) override;
	void Shutdown() override;
	void Update() override;
	void Draw() override;
};
