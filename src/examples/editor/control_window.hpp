#pragma once

#include "editor.hpp"



class ControlsWindow : public EditorWindow {
	friend Editor;
public:
	ControlsWindow(const char* _title);
	void Setup(Editor* _parent) override;
	void Shutdown() override;
	void Update() override;
	void Draw() override;
};
