#include "app.h"

// ** `raylib` Includes
#include <raylib.h>

// ** stdlib Includes
#include <stdio.h>
#include <math.h>

// ** imgui Includes
#include "imgui.h"
#include "imgui_impl_raylib.h"

// ** 
#include "editor/editor.hpp"


// GLFWwindow* g_mainWindow = nullptr;
static int s_screenWidth = 800;
static int s_screenHeight = 600;
static int s_selection = 0;
// static Sample* s_samples[s2_solverTypeCount];
// static Settings s_settings;
static bool s_rightMouseDown = false;
// static s2Vec2 s_clickPointWS = s2Vec2_zero;
static float s_windowScale = 2.0f;
static float s_framebufferScale = 1.0f;
static Editor* g_editor = nullptr;


AppState Create() {
    return AppState{  
    };
}


// Initialization
//--------------------------------------------------------------------------------------
void Initialize() {    

	// ** Raylib/Raylib Window
	// do not set the FLAG_WINDOW_HIGHDPI flag, that scales a low res framebuffer up to the native resolution.
	// use the native resolution and scale your geometry.
    bool fullscreen = false;
    int windowFlags;
	if (fullscreen) {
        windowFlags = FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_MAXIMIZED | FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI;
	}
	else {
        windowFlags = FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_MAXIMIZED | FLAG_WINDOW_RESIZABLE;
	}
	SetConfigFlags(windowFlags);
	InitWindow(s_screenWidth, s_screenHeight, "raylib-Extras [ImGui] example - Editor Example");	
	SetTargetFPS(144);
	SetExitKey(KEY_NULL);
	// Set Window Resolution to something Sensible
	int currentMonitor = GetCurrentMonitor();
	float aspectRatio = static_cast<float>(GetMonitorHeight(currentMonitor)) / GetMonitorWidth(currentMonitor);
	s_screenWidth  = static_cast<int>(GetMonitorWidth(currentMonitor) * 0.90f);
	s_screenHeight = static_cast<int>(s_screenWidth * aspectRatio);
	SetWindowSize(s_screenWidth, s_screenHeight);
	SetWindowPosition(
		abs(s_screenWidth - GetMonitorWidth(currentMonitor)) / 2,
		abs(s_screenHeight - GetMonitorHeight(currentMonitor)) / 2
	);

	// ** Dear ImGui
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigWindowsMoveFromTitleBarOnly = true;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	// Setup scaling
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(s_windowScale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
    style.FontScaleDpi = s_windowScale;        				// Set initial font scale. (using io.ConfigDpiScaleFonts=true makes this unnecessary. We leave both here for documentation purpose)
    io.ConfigDpiScaleFonts = true;          				// [Experimental] Automatically overwrite style.FontScaleDpi in Begin() when Monitor DPI changes. This will scale fonts but _NOT_ scale sizes/padding for now.
    io.ConfigDpiScaleViewports = true;      				// [Experimental] Scale Dear ImGui and Platform Windows when Monitor DPI changes.

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

	// Search for font file
	const char* fontPath1 = "data/droid_sans.ttf";
	const char* fontPath2 = "../data/droid_sans.ttf";
	const char* fontPath = nullptr;
	FILE* file1 = fopen(fontPath1, "rb");
	FILE* file2 = fopen(fontPath2, "rb");
	if (file1) {
		fontPath = fontPath1;
		fclose(file1);
	}

	if (file2) {
		fontPath = fontPath2;
		fclose(file2);
	}

	if (fontPath) {
		ImFontConfig fontConfig;
		fontConfig.RasterizerMultiply = s_windowScale * s_framebufferScale;
		ImGui::GetIO().Fonts->AddFontFromFileTTF(fontPath, 14.0f, &fontConfig);
	}

	// ** Editor
    g_editor = new Editor();
	g_editor->Initialize();
}
//-------------------------------------------------------------------------------------- end Initialize


// De-Initialization
//--------------------------------------------------------------------------------------   
void Destroy() {	
	ImGui_ImplRaylib_Shutdown();
	ImGui::DestroyContext();
	CloseWindow();                  // Close raylib window and OpenGL context
}
//-------------------------------------------------------------------------------------- end Destroy


bool ShouldClose() {
    return WindowShouldClose();
}


// Update Step
//----------------------------------------------------------------------------------
void Update() {    
    float deltatime = GetFrameTime();

	if (g_editor) {
		g_editor->Update();
	}	
}
//---------------------------------------------------------------------------------- end Update


// Render Step
//----------------------------------------------------------------------------------
void Draw() {
    BeginDrawing();
    ClearBackground(DARKGRAY);

    ImGui_ImplRaylib_NewFrame();
    ImGui::NewFrame();
    ImGui_ImplRaylib_ProcessEvents();
    ImGui::DockSpaceOverViewport(ImGui::GetWindowDockID(), ImGui::GetMainViewport(), 0);
    
	if (g_editor) { g_editor->Draw(); }

    ImGui::Render();
    ImGui_ImplRaylib_RenderDrawData(ImGui::GetDrawData());

    EndDrawing();    
}
//---------------------------------------------------------------------------------- end Draw