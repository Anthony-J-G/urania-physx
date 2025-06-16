#pragma once

#include "editor.hpp"



class SceneListWindow : public EditorWindow {

public:
	void Setup(Editor* editor_ref) override;
	void Shutdown() override;
	void Update() override;
	void Draw() override;
};
