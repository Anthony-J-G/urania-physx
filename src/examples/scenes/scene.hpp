// SPDX-FileCopyrightText: 2024 Erin Catto
// SPDX-License-Identifier: MIT

#pragma once

#include <engine/types.h>

#include <stdlib.h>

struct Settings;
class Test;

#ifdef _DEBUG
constexpr bool g_SceneDebug = true;
#else
constexpr bool g_SceneDebug = false;
#endif

#define RAND_LIMIT 32767

/// Random number in range [-1,1]
inline float RandomFloat() {
	float r = (float)(rand() & (RAND_LIMIT));
	r /= RAND_LIMIT;
	r = 2.0f * r - 1.0f;
	return r;
}

/// Random floating point number in range [lo, hi]
inline float RandomFloat(float lo, float hi) {
	float r = (float)(rand() & (RAND_LIMIT));
	r /= RAND_LIMIT;
	r = (hi - lo) * r + lo;
	return r;
}

class Scene {
  public:
	Scene(const Settings& settings, int solverType);
	virtual ~Scene();

	void DrawTitle(Settings& settings, const char* string);
	virtual void Step(Settings& settings, s2Color bodyColor);
	virtual void UpdateUI()
	{
	}
	virtual void Keyboard(int)
	{
	}
	virtual void KeyboardUp(int)
	{
	}
	virtual void MouseDown(s2Vec2 p, int button, int mod);
	virtual void MouseUp(s2Vec2 p, int button);
	virtual void MouseMove(s2Vec2 p);

	int m_solverType;
	s2BodyId m_groundBodyId;
	s2WorldId m_worldId;
	s2JointId m_mouseJointId;
	int m_stepCount;
	int m_textIncrement;
};

typedef Scene* SceneCreateFn(const Settings& settings, int solverType);

int RegisterScene(const char* category, const char* name, SceneCreateFn* fn);

struct SceneEntry {
	const char* category;
	const char* name;
	SceneCreateFn* createFn;
};

#define MAX_SCENES 256
extern SceneEntry g_sceneEntries[MAX_SCENES];
extern int g_sceneCount;
