#include "editor.h"

#include <raylib.h>
#include <raymath.h>

#include <imgui.h>

#include "rlImGui.h"


// Utility Functions
//----------------------------------------------------------------------------------
// Forward Declarations
float 	ScaleToDPI(float value);
int 	ScaleToDPI(int value);

float ScaleToDPI(float value) {
    return GetWindowScaleDPI().x * value;
}


int ScaleToDPI(int value) {
    return int(GetWindowScaleDPI().x * value);
}
//---------------------------------------------------------------------------------- End Utility Functions

// Editor
//----------------------------------------------------------------------------------
Editor::Editor() {
	ImageViewerWindow 	img_viewer;
	SceneViewWindow 	scene_view;
}

void Editor::CreateMainMenuBar() {
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
				should_quit = true;

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Window"))
		{
			ImGui::MenuItem("ImGui Demo", nullptr, &ImGuiDemoOpen);
			ImGui::MenuItem("Image Viewer", nullptr, &ImageViewer.Open);
			ImGui::MenuItem("3D View", nullptr, &SceneView.Open);

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}


// Image Viewer Window
//----------------------------------------------------------------------------------
void ImageViewerWindow::Setup() {
	Camera.zoom = 1;
	Camera.target.x = 0;
	Camera.target.y = 0;
	Camera.rotation = 0;
	Camera.offset.x = GetScreenWidth() / 2.0f;
	Camera.offset.y = GetScreenHeight() / 2.0f;

	ViewTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
	ImageTexture = LoadTexture("resources/parrots.png");

	UpdateRenderTexture();
}


void ImageViewerWindow::Show() {
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::SetNextWindowSizeConstraints(ImVec2(ScaleToDPI(400.0f), ScaleToDPI(400.0f)), ImVec2(float(GetScreenWidth()), float(GetScreenHeight())));
	Focused = false;

	if (ImGui::Begin("Image Viewer", &Open, ImGuiWindowFlags_NoScrollbar)) {
		// save off the screen space content rectangle
		ContentRect = {
			ImGui::GetWindowPos().x + ImGui::GetCursorScreenPos().x, ImGui::GetWindowPos().y + ImGui::GetCursorScreenPos().y, ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y 
		};

		Focused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);

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
	if (!Open) {
		return;
	}		

	if (IsWindowResized()) {
		UnloadRenderTexture(ViewTexture);
		ViewTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

		Camera.offset.x = GetScreenWidth() / 2.0f;
		Camera.offset.y = GetScreenHeight() / 2.0f;
	}

	Vector2 mousePos = GetMousePosition();

	if (Focused) {
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


// Scene View Window
//----------------------------------------------------------------------------------
void SceneViewWindow::Setup() {
	ViewTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

	Camera.fovy = 45;
	Camera.up.y = 1;
	Camera.position.y = 3;
	Camera.position.z = -25;

	Image img = GenImageChecked(ScaleToDPI(256), ScaleToDPI(256), ScaleToDPI(32), ScaleToDPI(32), DARKGRAY, WHITE);
	GridTexture = LoadTextureFromImage(img);
	UnloadImage(img);
	GenTextureMipmaps(&GridTexture);
	SetTextureFilter(GridTexture, TEXTURE_FILTER_ANISOTROPIC_16X);
	SetTextureWrap(GridTexture, TEXTURE_WRAP_CLAMP);
}


void SceneViewWindow::Shutdown() {
	UnloadRenderTexture(ViewTexture);
	UnloadTexture(GridTexture);
}


void SceneViewWindow::Show() {
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::SetNextWindowSizeConstraints(ImVec2(ScaleToDPI(400.0f), ScaleToDPI(400.0f)), ImVec2((float)GetScreenWidth(), (float)GetScreenHeight()));

	if (ImGui::Begin("3D View", &Open, ImGuiWindowFlags_NoScrollbar))
	{
		Focused = ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows);
		// draw the view
		rlImGuiImageRenderTextureFit(&ViewTexture, true);
	}
	ImGui::End();
	ImGui::PopStyleVar();
}


void SceneViewWindow::Update() {
	if (!Open)
		return;

	if (IsWindowResized())
	{
		UnloadRenderTexture(ViewTexture);
		ViewTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
	}

	float period = 10;
	float magnitude = 25;

	Camera.position.x = sinf(float(GetTime() / period)) * magnitude;

	BeginTextureMode(ViewTexture);
	ClearBackground(SKYBLUE);

	BeginMode3D(Camera);

	// grid of cube trees on a plane to make a "world"
	DrawPlane(Vector3{ 0, 0, 0 }, Vector2{ 50, 50 }, BEIGE); // simple world plane
	float spacing = 4;
	int count = 5;

	for (float x = -count * spacing; x <= count * spacing; x += spacing)
	{
		for (float z = -count * spacing; z <= count * spacing; z += spacing)
		{
			Vector3 pos = { x, 0.5f, z };

			Vector3 min = { x - 0.5f,0,z - 0.5f };
			Vector3 max = { x + 0.5f,1,z + 0.5f };

			DrawCube(Vector3{ x, 1.5f, z }, 1, 1, 1, GREEN);
			DrawCube(Vector3{ x, 0.5f, z }, 0.25f, 1, 0.25f, BROWN);
		}
	}

	EndMode3D();
	EndTextureMode();
}
//---------------------------------------------------------------------------------- End Scene View Window

