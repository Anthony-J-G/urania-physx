#define _CRT_SECURE_NO_WARNINGS
#include "settings.hpp"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char* fileName = "settings.json";
Settings g_settings = Settings{};

// Load a file. You must free the character array.
static bool ReadFile(char*& data, int& size, const char* filename) {
	FILE* file = fopen(filename, "rb");
	if (file == nullptr) {
		return false;
	}

	fseek(file, 0, SEEK_END);
	size = ftell(file);
	fseek(file, 0, SEEK_SET);

	if (size == 0) {
		return false;
	}

	data = (char*)malloc(size + 1);
	fread(data, size, 1, file);
	fclose(file);
	data[size] = 0;

	return true;
}


void Settings::Save(const Settings& settings) {
	FILE* file = fopen(fileName, "w");

	// TODO(anthony-j-g): write struct to json string and then to file
	fprintf(file, "{\n");
	fprintf(file, "  \"sampleIndex\": %d,\n", settings.sampleIndex);
	fprintf(file, "  \"drawShapes\": %s,\n", settings.drawShapes ? "true" : "false");
	fprintf(file, "  \"drawJoints\": %s,\n", settings.drawJoints ? "true" : "false");
	fprintf(file, "  \"drawAABBs\": %s,\n", settings.drawAABBs ? "true" : "false");
	fprintf(file, "  \"drawContactPoints\": %s,\n", settings.drawContactPoints ? "true" : "false");
	fprintf(file, "  \"drawContactNormals\": %s,\n", settings.drawContactNormals ? "true" : "false");
	fprintf(file, "  \"drawContactImpulse\": %s,\n", settings.drawContactImpulse ? "true" : "false");
	fprintf(file, "  \"drawFrictionImpulse\": %s,\n", settings.drawFrictionImpulse ? "true" : "false");
	fprintf(file, "  \"drawMass\": %s,\n", settings.drawMass ? "true" : "false");
	fprintf(file, "  \"drawStats\": %s,\n", settings.drawStats ? "true" : "false");
	fprintf(file, "  \"enableWarmStarting\": %s\n", settings.enableWarmStarting ? "true" : "false");
	fprintf(file, "}\n");
	fclose(file);
}


#define MAX_TOKENS 32


void Settings::Load(Settings& settings) {
	char* data = nullptr;
	int size = 0;
	bool found = ReadFile(data, size, fileName);
	if (found ==  false) {
		return;
	}

	json obj;
	try {
		obj = json::parse(data);
	} catch (int errorCode) {
		printf("[setting.json] Caught exception in json parse fn, terminating with code %d\n", EXIT_FAILURE);
		exit(EXIT_FAILURE);
	}	

	printf("%s\n", obj.dump().c_str());

	// TODO(anthony-j-g): write json data to struct
}
