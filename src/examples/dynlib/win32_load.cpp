#include "common_api.hpp"

#include <engine/scene.hpp>

#include <minwindef.h>
#include <stdio.h>
#include <libloaderapi.h>
#include <windows.h>



void LoadEngineLibrary(EngineLibrary& lib) {
	void* functions[ENGINE_API_SIZE] = {};
	memset(functions, 0, ENGINE_API_SIZE);

	if (lib.handle != nullptr) {
		printf("ERROR: [win32] attempting to load library `" ENGINE_LIB_PATH "` into non-null handle\n");
		return;
	}
	lib.is_fully_loaded = false;

	lib.handle = static_cast<void *>(LoadLibrary(ENGINE_LIB_PATH));
	if (lib.handle == NULL) {
		printf("ERROR: [win32] library `" ENGINE_LIB_PATH "` could not be loaded\n");
		return;
	} else {
		printf("INFO: [win32] library `" ENGINE_LIB_PATH "` found by application. Attempting load.\n");
	}
	for (size_t i = 0; i < ENGINE_API_SIZE; i++) {
		functions[i] = (void*)GetProcAddress((HMODULE)lib.handle, "");
	}

	lib.api.EngineInit 	= (void (*)()) GetProcAddress((HMODULE)lib.handle,
		"InitEngine"
	);
	lib.api.RegisterScene = (bool (*)(Scene_API*)) GetProcAddress((HMODULE)lib.handle,
		"RegisterScene"
	);
	lib.api.GetScene  	= (Scene_API* (*)(const char*)) GetProcAddress((HMODULE)lib.handle,
		"GetScene"
	);
	lib.api.GetSceneNames  	= (std::vector<const char*> (*)()) GetProcAddress((HMODULE)lib.handle,
		"_Z13GetSceneNamesv"
	);

	lib.is_fully_loaded = false;
	// lib.is_fully_loaded = DidLibraryLoadSucceed(ENGINE_API_SIZE, functions);
	isEngineLoadSuccess(lib);
	if (!lib.is_fully_loaded) {
		printf("WARNING: [win32] library `" ENGINE_LIB_PATH "` partial load.\n");
	} else {
		printf("INFO: [win32] library `" ENGINE_LIB_PATH "` loaded successfully.\n");
	}

	if (lib.is_fully_loaded) {
		lib.api.EngineInit();
	}
}


void BuildEngineApi(EngineApi& api, void *funcitons[]) {

}


void UnloadEngineLibrary(EngineLibrary& lib) {
	FreeLibrary(static_cast<HMODULE>(lib.handle));
	lib.handle = nullptr;
	lib.is_fully_loaded = false;
}

