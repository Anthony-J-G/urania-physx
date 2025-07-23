#include "scene.hpp"

#include <math.h>
#include <stdio.h>



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
static int sampleFarChain = RegisterScene("Gravity", "N-Body", NBody::Create);