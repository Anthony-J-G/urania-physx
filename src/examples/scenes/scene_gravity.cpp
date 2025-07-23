#include "scene.hpp"

#include <math.h>
#include <stdio.h>


class MoonEarth : public Scene {
public:
	enum {
		e_count = 40
	};

	MoonEarth(const Settings& settings, int solverType)
		: Scene(settings, solverType)
	{
	}

	static Scene* Create(const Settings& settings, int solverType) {
		return new MoonEarth(settings, solverType);
	}
};
static int exampleMoonEarth = RegisterScene("Gravity", "Moon/Earth System", MoonEarth::Create);


class NBody : public Scene {
public:
	enum {
		e_count = 40
	};

	NBody(const Settings& settings, int solverType)
		: Scene(settings, solverType)
	{
	}

	static Scene* Create(const Settings& settings, int solverType) {
		return new NBody(settings, solverType);
	}
};
static int exampleNBody = RegisterScene("Gravity", "N-Body", NBody::Create);