#include "sample.hpp"
#include "scene.hpp"

#include <math.h>
#include <raylib.h>
#include <rlgl.h>
#include <raymath.h>


//------------------------------------------------------------------------------------------
// Types and Structures Definition
//------------------------------------------------------------------------------------------
typedef enum GameScreen { LOGO = 0, TITLE, GAMEPLAY, ENDING } GameScreen;

static GameScreen g_currentScreen   = LOGO;
static int g_sceneFramesCounter     = 0;


void InitializeSample() {
     g_currentScreen = LOGO;

    // TODO: Initialize all required variables and load all required data here!

    g_sceneFramesCounter = 0;          // Useful to count frames
}


void ShutdownSample() {
}


void Update(float deltatime) {
    switch (g_currentScreen) {
        case LOGO: {
            // TODO: Update LOGO screen variables here!

            g_sceneFramesCounter++;    // Count frames

            // Wait for 2 seconds (120 frames) before jumping to TITLE screen
            if (g_sceneFramesCounter > 120) {
                g_currentScreen = TITLE;
            }
        } break;
        case TITLE: {
            // TODO: Update TITLE screen variables here!

            // Press enter to change to GAMEPLAY screen
            if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
            {
                g_currentScreen = GAMEPLAY;
            }
        } break;
        case GAMEPLAY: {
            // TODO: Update GAMEPLAY screen variables here!

            // Press enter to change to ENDING screen
            if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) {
                g_currentScreen = ENDING;
            }
        } break;
        case ENDING: {
            // TODO: Update ENDING screen variables here!

            // Press enter to return to TITLE screen
            if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) {
                g_currentScreen = TITLE;
            }
        } break;
        default: break;
    }
}


void Render() {
     switch(g_currentScreen) {
        case LOGO: {
            // TODO: Draw LOGO screen here!
            DrawText("LOGO SCREEN", 20, 20, 40, LIGHTGRAY);
            DrawText("WAIT for 2 SECONDS...", 290, 220, 20, GRAY);

        } break;
        case TITLE: {
            // TODO: Draw TITLE screen here!
            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), GREEN);
            DrawText("TITLE SCREEN", 20, 20, 40, DARKGREEN);
            DrawText("PRESS ENTER or TAP to JUMP to GAMEPLAY SCREEN", 120, 220, 20, DARKGREEN);

        } break;
        case GAMEPLAY: {
            // TODO: Draw GAMEPLAY screen here!
            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), PURPLE);
            DrawText("GAMEPLAY SCREEN", 20, 20, 40, MAROON);
            DrawText("PRESS ENTER or TAP to JUMP to ENDING SCREEN", 130, 220, 20, MAROON);

        } break;
        case ENDING: {
            // TODO: Draw ENDING screen here!
            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLUE);
            DrawText("ENDING SCREEN", 20, 20, 40, DARKBLUE);
            DrawText("PRESS ENTER or TAP to RETURN to TITLE SCREEN", 120, 220, 20, DARKBLUE);

        } break;
        default: break;
    }
}


Scene_API GenerateSampleScene() {
    return Scene_API{
        .name = "Sample",
        .initialize_fn = InitializeSample,
        .shutdown_fn = ShutdownSample,
        .update_fn = Update,
        .render_fn = Render,        
    };
}
