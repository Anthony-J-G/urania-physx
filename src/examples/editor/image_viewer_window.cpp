#include "image_viewer_window.hpp"

// ** `raylib` Includes
#include <raylib.h>
#include <raymath.h>

// ** `Dear ImGui` Includes
#include <imgui.h>

#include "editor.hpp"
#include "rlImGui.h"


// Image Viewer Window
//----------------------------------------------------------------------------------
ImageViewerWindow::ImageViewerWindow(const char* _title) : EditorWindow(_title) {
}


void ImageViewerWindow::Setup(Editor* editor_ref) {
	Camera.zoom = 1;
	Camera.target.x = 0;
	Camera.target.y = 0;
	Camera.rotation = 0;
	Camera.offset.x = GetScreenWidth() / 2.0f;
	Camera.offset.y = GetScreenHeight() / 2.0f;

	is_open = true;
	ViewTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
	ImageTexture = LoadTexture("resources/parrots.png");

	UpdateRenderTexture();
}


void ImageViewerWindow::Draw() {
	if (!is_open) { return; }

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::SetNextWindowSizeConstraints(ImVec2(ScaleToDPI(400.0f), ScaleToDPI(400.0f)), ImVec2(float(GetScreenWidth()), float(GetScreenHeight())));
	is_focused = false;

	if (ImGui::Begin("Image Viewer", &is_open, ImGuiWindowFlags_NoScrollbar)) {
		// save off the screen space content rectangle
		ContentRect = {
			ImGui::GetWindowPos().x + ImGui::GetCursorScreenPos().x, ImGui::GetWindowPos().y + ImGui::GetCursorScreenPos().y, ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y 
		};

		is_focused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);

		ImVec2 size = ImGui::GetContentRegionAvail();

		// center the scratch pad in the view
		Rectangle viewRect = { 0 };
		viewRect.x = ViewTexture.texture.width / 2 - size.x / 2;
		viewRect.y = ViewTexture.texture.height / 2 - size.y / 2;
		viewRect.width = size.x;
		viewRect.height = -size.y;

		if (ImGui::BeginChild("Toolbar", ImVec2(ImGui::GetContentRegionAvail().x, 25))) {
			ImGui::SetCursorPosX(2);
			ImGui::SetCursorPosY(3);

			if (ImGui::Button("None")) {
				CurrentToolMode = ToolMode::None;
			}
			ImGui::SameLine();

			if (ImGui::Button("Move")) {
				CurrentToolMode = ToolMode::Move;
			}

			ImGui::SameLine();
			switch (CurrentToolMode) {
				case ToolMode::None:
					ImGui::TextUnformatted("No Tool");
					break;
				case ToolMode::Move:
					ImGui::TextUnformatted("Move Tool");
					break;
				default:
					break;
			}

			ImGui::SameLine();
			ImGui::TextUnformatted(TextFormat("camera target X%f Y%f", Camera.target.x, Camera.target.y));
			ImGui::EndChild();
		}

		rlImGuiImageRect(&ViewTexture.texture, int(size.x), int(size.y), viewRect);	
	}
	ImGui::End();
	ImGui::PopStyleVar();
}


void ImageViewerWindow::Update() {
	if (!is_open) {
		return;
	}		

	if (IsWindowResized()) {
		UnloadRenderTexture(ViewTexture);
		ViewTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

		Camera.offset.x = GetScreenWidth() / 2.0f;
		Camera.offset.y = GetScreenHeight() / 2.0f;
	}

	Vector2 mousePos = GetMousePosition();

	if (is_focused) {
		if (CurrentToolMode == ToolMode::Move) {
			// only do this tool when the mouse is in the content area of the window
			if (IsMouseButtonDown(0) && CheckCollisionPointRec(mousePos, ContentRect)) {
				if (!Dragging) {
					LastMousePos = mousePos;
					LastTarget = Camera.target;
				}
				Dragging = true;
				Vector2 mouseDelta = Vector2Subtract(LastMousePos, mousePos);

				mouseDelta.x /= Camera.zoom;
				mouseDelta.y /= Camera.zoom;
				Camera.target = Vector2Add(LastTarget, mouseDelta);

				DirtyScene = true;

			} else {
				Dragging = false;
			}
		}
	} else {
		Dragging = false;
	}

	if (DirtyScene) {
		DirtyScene = false;
		UpdateRenderTexture();
	}
}


void ImageViewerWindow::UpdateRenderTexture() {
	BeginTextureMode(ViewTexture);
	ClearBackground(BLUE);

	// camera with our view offset with a world origin of 0,0
	BeginMode2D(Camera);

	// center the image at 0,0
	DrawTexture(ImageTexture, ImageTexture.width / -2, ImageTexture.height / -2, WHITE);

	EndMode2D();
	EndTextureMode();
}


void ImageViewerWindow::Shutdown() {
	UnloadRenderTexture(ViewTexture);
	UnloadTexture(ImageTexture);
}
//---------------------------------------------------------------------------------- End Image Viewer Window