#include "editor.hpp"

#include <raylib.h>
#include <raymath.h>

#include <imgui.h>

#include "rlImGui.h"

// ** `win32` Includes
#if defined(_WIN32) || defined(_WIN64)
#	include "../win32_dynamic_api.hpp"
#else // Win32 or Win64
// TODO(anthony-j-g): enable this for Linux and OSX
#	error "Dynamic reloading not supported in OS other than Windows!"
#endif


// Editor Window Enumeration
//----------------------------------------------------------------------------------
enum EditorWindowIndex {
	EDITOR_IMAGE_VIEWER = 0,
	EDITOR_SCENE_VIEWER = 1,
	EDITOR_SCENE_LIST	= 2,
};
//---------------------------------------------------------------------------------- End Utility Functions

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
}


void Editor::Update() {
	for (EditorWindow* window : editor_windows) {
		window->Update();
	}
}


void Editor::Draw() {
	DrawMainMenuBar();

	for (EditorWindow* window : editor_windows) {
		window->Draw();
	}

	if (show_imgui_demo) {
		ImGui::ShowDemoWindow(&show_imgui_demo);
	}	
}


void Editor::Initialize() {
	// Load Dynamic APIs
	win32::LoadDynamicLibrary(physics_api);

	auto img_viewer			= new ImageViewerWindow();
	auto scene_view 		= new SceneViewWindow();
	auto scene_list 		= new SceneListWindow();

	editor_windows.reserve(3);
	editor_windows.push_back(static_cast<EditorWindow *>(img_viewer));
	editor_windows.push_back(static_cast<EditorWindow *>(scene_view));
	editor_windows.push_back(static_cast<EditorWindow *>(scene_list));

	for (EditorWindow* window: editor_windows) {
		window->Setup(this);
	}
}


void Editor::Shutdown() {
	for (EditorWindow* window : editor_windows) {
		delete window;
	}
	editor_windows.clear();
}


Editor::~Editor() {
	// Ensure resources are cleaned up on destruction
	Shutdown();
}


void Editor::DrawMainMenuBar() {
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Exit")) {
				should_quit = true;
			}				
			ImGui::EndMenu();
		}		

		if (ImGui::BeginMenu("Window")) {
			ImGui::MenuItem("ImGui Demo", nullptr, &show_imgui_demo);
			// TODO(anthony-j-g): Fix these so they properly reference the window's boolean
			// ImGui::MenuItem("Image Viewer", nullptr, &editor_windows[EDITOR_IMAGE_VIEWER]);
			// ImGui::MenuItem("3D View", nullptr, &editor_windows[EDITOR_SCENE_VIEWER]);

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}
//----------------------------------------------------------------------------------


// Image Viewer Window
//----------------------------------------------------------------------------------
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


// Scene View Window
//----------------------------------------------------------------------------------
void SceneViewWindow::Setup(Editor* editor_ref) {
	parent = editor_ref;
	parent->current_scene = parent->physics_api.GetScene("OscillatingCircle");
	ViewTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

	Camera.fovy = 45;
	Camera.up.y = 1;
	Camera.position.y = 3;
	Camera.position.z = -25;

	is_open = true;
	Image img = GenImageChecked(ScaleToDPI(256), ScaleToDPI(256), ScaleToDPI(32), ScaleToDPI(32), DARKGRAY, WHITE);
	GridTexture = LoadTextureFromImage(img);
	UnloadImage(img);
	GenTextureMipmaps(&GridTexture);
	SetTextureFilter(GridTexture, TEXTURE_FILTER_ANISOTROPIC_16X);
	SetTextureWrap(GridTexture, TEXTURE_WRAP_CLAMP);
}


void SceneViewWindow::Shutdown() {
	parent->physics_api.Scene__Shutdown();
	UnloadRenderTexture(ViewTexture);
	UnloadTexture(GridTexture);
}


void SceneViewWindow::Update() {
	if (!is_open) { return; }

	if (IsWindowResized()) {
		UnloadRenderTexture(ViewTexture);
		ViewTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
	}	
	parent->physics_api.Scene__Update(GetFrameTime());
	float period = 10;
	float magnitude = 25;

	Camera.position.x = sinf(float(GetTime() / period)) * magnitude;

	BeginTextureMode(ViewTexture);
	ClearBackground(SKYBLUE);

	parent->physics_api.Scene__Render();
	// BeginMode3D(Camera);
// 
	// // grid of cube trees on a plane to make a "world"
	// DrawPlane(Vector3{ 0, 0, 0 }, Vector2{ 50, 50 }, BEIGE); // simple world plane
	// float spacing = 4;
	// int count = 5;
// 
	// for (float x = -count * spacing; x <= count * spacing; x += spacing) {
	// 	for (float z = -count * spacing; z <= count * spacing; z += spacing) {
	// 		Vector3 pos = { x, 0.5f, z };
// 
	// 		Vector3 min = { x - 0.5f,0,z - 0.5f };
	// 		Vector3 max = { x + 0.5f,1,z + 0.5f };
// 
	// 		DrawCube(Vector3{ x, 1.5f, z }, 1, 1, 1, GREEN);
	// 		DrawCube(Vector3{ x, 0.5f, z }, 0.25f, 1, 0.25f, BROWN);
	// 	}
	// }
// 
	// EndMode3D();
	EndTextureMode();
}


void SceneViewWindow::Draw() {
	if (!is_open) { return; }
		
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::SetNextWindowSizeConstraints(ImVec2(ScaleToDPI(400.0f), ScaleToDPI(400.0f)), ImVec2((float)GetScreenWidth(), (float)GetScreenHeight()));

	if (ImGui::Begin("3D View", &is_open, ImGuiWindowFlags_NoScrollbar)) {
		is_focused = ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows);
		// draw the view
		rlImGuiImageRenderTextureFit(&ViewTexture, true);
	}
	ImGui::End();
	ImGui::PopStyleVar();
}
//---------------------------------------------------------------------------------- End Scene View Window


// Scene List Window
//----------------------------------------------------------------------------------
void SceneListWindow::Setup(Editor* editor_ref) {
	is_open = true;
}


void SceneListWindow::Shutdown() {
}


void SceneListWindow::Update() {
}


void SceneListWindow::Draw() {
	if (!is_open) { return; }

	ImGui::Begin("Scene List");
	ImGui::End();
}
//---------------------------------------------------------------------------------- End Scene List Window

