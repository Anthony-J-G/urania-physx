#include "scene.hpp"

// #include "settings.h"
#include <stdio.h>
#include <string.h>



Scene::Scene(const Settings& settings, int solverType) {	
}


Scene::~Scene() {
}

void Scene::DrawTitle(Settings& settings, const char* string) {	
}


struct QueryContext {
	s2Vec2 point;
	s2BodyId bodyId = s2_nullBodyId;
};


bool QueryCallback(s2ShapeId shapeId, void* context) {
	return true;
}


void Scene::MouseDown(s2Vec2 p, int button, int mod) {	
}


void Scene::MouseUp(s2Vec2 p, int button) {	
}


void Scene::MouseMove(s2Vec2 p) {	
}


void Scene::Step(Settings& settings, s2Color bodyColor) {	
}

SceneEntry g_sceneEntries[MAX_SCENES] = {{nullptr}};
int g_sceneCount = 0;

int RegisterScene(const char* category, const char* name, SceneCreateFn* fn) {
	int index = g_sceneCount;
	if (index < MAX_SCENES) {
		g_sceneEntries[index] = {category, name, fn};
		++g_sceneCount;
		return index;
	}

	return -1;
}
