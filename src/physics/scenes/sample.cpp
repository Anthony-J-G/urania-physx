#include "sample.hpp"

#include <math.h>
#include <raylib.h>
#include <rlgl.h>
#include <raymath.h>


// Function Declarations
void CameraModeFollow(Camera2D& camera, const Vector2& target, const Vector2& frame, const float deltatime);
void CameraModeFollowSmoothed(Camera2D& camera, const Vector2& target, const Vector2& frame, const float deltatime);
void CameraModeBoundsPush(Camera2D& camera, const Vector2& target, const Vector2& frame, const float deltatime);


struct SceneObjects {
    Camera2D camera;
    Vector2 circle_pos;
    float circle_speed;
};
static auto g_SceneObjs = SceneObjects{};



OscillatingCircleScene::OscillatingCircleScene() {
    g_SceneObjs.camera = Camera2D{
        .offset = {GetScreenWidth()/2.0f,GetScreenHeight()/2.0f},     // Camera offset (displacement from target)
        .target = {0, 0},                                   // Camera target (rotation and zoom origin)
        .rotation = 0,                                            // Camera rotation in degrees
        .zoom = 1.0f,                                             // Camera zoom (scaling), should be 1.0f by default
    };
    g_SceneObjs.circle_pos = {0.0, 0.0};
    g_SceneObjs.circle_speed = 250.0f;
}


void OscillatingCircleScene::Init() {

}


void OscillatingCircleScene::Shutdown() {
}


void OscillatingCircleScene::Update(float deltatime) {
    static int frame_number = 0;
    static float t = 0;
    t += deltatime;
    frame_number += 1;

    const int screenWidth =  GetScreenWidth();
    const int screenHeight = GetScreenHeight();

    if (IsKeyDown(KEY_RIGHT)) g_SceneObjs.circle_pos.x += g_SceneObjs.circle_speed * deltatime;
    if (IsKeyDown(KEY_LEFT )) g_SceneObjs.circle_pos.x -= g_SceneObjs.circle_speed * deltatime;        
    if (IsKeyDown(KEY_UP   )) g_SceneObjs.circle_pos.y -= g_SceneObjs.circle_speed * deltatime;
    if (IsKeyDown(KEY_DOWN )) g_SceneObjs.circle_pos.y += g_SceneObjs.circle_speed * deltatime;

    // Calculate Oscilation
    g_SceneObjs.circle_pos.y = 500 * sin(0.4 * t);
    g_SceneObjs.circle_pos.x = 500 * cos(0.4 * t);
    
    // CameraModeFollow(g_SceneObjs.camera, g_SceneObjs.circle_pos, Vector2{ 1.0f * screenWidth, 1.0f * screenHeight }, deltatime);
    CameraModeFollowSmoothed(g_SceneObjs.camera, g_SceneObjs.circle_pos, Vector2{ 1.0f * screenWidth, 1.0f * screenHeight }, deltatime);
    // CameraModeBoundsPush(g_SceneObjs.camera, g_SceneObjs.circle_pos, Vector2{ 1.0f * screenWidth, 1.0f * screenHeight }, deltatime);
}


void OscillatingCircleScene::Render() {
    BeginMode2D(g_SceneObjs.camera); {
        // Draw the 3d grid, rotated 90 degrees and centered around 0,0 
        // just so we have something in the XY plane
        rlPushMatrix();
            rlTranslatef(0, 25*50, 0);
            rlRotatef(90, 1, 0, 0);
            DrawGrid(100, 100);
        rlPopMatrix();

        // Draw a reference circle
        DrawCircleV(g_SceneObjs.circle_pos, 50, MAROON);
        DrawCircleV({0.0, 0.0}, 5, SKYBLUE);
    }
    EndMode2D();
}


void CameraModeFollow(Camera2D& camera, const Vector2& target, const Vector2& frame, const float deltatime) {
    camera.offset = Vector2{ frame.x/2.0f, frame.y / 2.0f };
    camera.target = target;
}


void CameraModeFollowSmoothed(Camera2D& camera, const Vector2& target, const Vector2& frame, const float deltatime) {
    static float minSpeed = 30;
    static float minEffectLength = 10;
    static float fractionSpeed = 0.8f;

    camera.offset = Vector2{ frame.x/2.0f, frame.y / 2.0f };
    Vector2 diff = Vector2Subtract(target, camera.target);
    float length = Vector2Length(diff);

    if (length > minEffectLength) {
        float speed = fmaxf(fractionSpeed*length, minSpeed);
        camera.target = Vector2Add(camera.target, Vector2Scale(diff, speed * deltatime / length));
    }
}


void CameraModeBoundsPush(Camera2D& camera, const Vector2& target, const Vector2& frame, const float deltatime) {
    static Vector2 bbox = { 20.0f, 20.0f };

    Vector2 bboxWorldMin = GetScreenToWorld2D((Vector2){ (1 - bbox.x)*0.5f*frame.x, (1 - bbox.y)*0.5f*frame.y }, camera);
    Vector2 bboxWorldMax = GetScreenToWorld2D((Vector2){ (1 + bbox.x)*0.5f*frame.x, (1 + bbox.y)*0.5f*frame.y }, camera);
    camera.offset = (Vector2){ (1 - bbox.x)*0.5f * frame.x, (1 - bbox.y)*0.5f*frame.y };

    if (target.x < bboxWorldMin.x) camera.target.x = target.x;
    if (target.y < bboxWorldMin.y) camera.target.y = target.y;
    if (target.x > bboxWorldMax.x) camera.target.x = bboxWorldMin.x + (target.x - bboxWorldMax.x);
    if (target.y > bboxWorldMax.y) camera.target.y = bboxWorldMin.y + (target.y - bboxWorldMax.y);
}

