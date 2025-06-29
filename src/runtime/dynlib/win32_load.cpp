#include "win32_load.hpp"
#include "common_api.hpp"

#include <engine/scene.hpp>

#include <stdio.h>
#include <libloaderapi.h>
#include <windows.h>

namespace win32 {

HMODULE g_EngineLibraryHandle;               // Handle to DLL


void LoadEngineLibrary(EngineLibrary& lib) {
	void* functions[ENGINE_API_SIZE] = {};
	memset(functions, 0, ENGINE_API_SIZE);

	if (lib.handle != nullptr) {
		printf("ERROR: [win32] attempting to load library `" ENGINE_LIB_NAME_OS_WINDOWS "` into non-null handle\n");
		return;
	}
	lib.is_fully_loaded = false;

	lib.handle = static_cast<void *>(LoadLibrary(ENGINE_LIB_NAME_OS_WINDOWS));
	if (g_EngineLibraryHandle == NULL) {
		printf("ERROR: [win32] library `" ENGINE_LIB_NAME_OS_WINDOWS "` could not be loaded\n");
		return;
	} else {
		printf("INFO: [win32] library `" ENGINE_LIB_NAME_OS_WINDOWS "` found by application. Attempting load.\n");
	}
	for (size_t i = 0; i < ENGINE_API_SIZE; i++) {

	}

	lib.api.EngineInit 	= (void (*)()) GetProcAddress(g_EngineLibraryHandle,
		"InitEngine"
	);
	lib.api.RegisterScene = (bool (*)(Scene_API*)) GetProcAddress(g_EngineLibraryHandle,
		"RegisterScene"
	);
	lib.api.GetScene  	= (Scene_API* (*)(const char*)) GetProcAddress(g_EngineLibraryHandle,
		"GetScene"
	);
	lib.api.GetSceneNames  	= (std::vector<const char*> (*)()) GetProcAddress(g_EngineLibraryHandle,
		"_Z13GetSceneNamesv"
	);

	lib.is_fully_loaded = false;
	// lib.is_fully_loaded = DidLibraryLoadSucceed(ENGINE_API_SIZE, functions);
	isEngineLoadSuccess(lib);
	if (!lib.is_fully_loaded) {
		printf("WARNING: [win32] library `" ENGINE_LIB_NAME_OS_WINDOWS "` partial load.\n");
	} else {
		printf("INFO: [win32] library `" ENGINE_LIB_NAME_OS_WINDOWS "` loaded successfully.\n");
	}

	if (lib.is_fully_loaded) {
		lib.api.EngineInit();
	}
}


} // end namespace win32


