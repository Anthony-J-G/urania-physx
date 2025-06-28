#include "win32_load.hpp"
#include "common_api.hpp"

#include <stdio.h>
#include <libloaderapi.h>
#include <windows.h>

namespace win32 {

HMODULE g_EngineLibraryHandle;               // Handle to DLL


void LoadEngineLibrary(EngineLibrary& api) {
	g_EngineLibraryHandle = LoadLibrary(ENGINE_LIB_NAME_OS_WINDOWS);
	if (g_EngineLibraryHandle == NULL) {
		printf("ERROR: [win32] library `" ENGINE_LIB_NAME_OS_WINDOWS "` could not be loaded\n");
		return;
	} else {
		printf("INFO: [win32] library `" ENGINE_LIB_NAME_OS_WINDOWS "` found by application. Attempting load.\n");
	}

	api.is_loaded = false;

	api.EngineInit 	= (void (*)()) GetProcAddress(g_EngineLibraryHandle,
		"_Z10InitEnginev"
	);
	api.GetScene  	= (Scene_API* (*)(const char*)) GetProcAddress(g_EngineLibraryHandle,
		"GetScene"
	);
	api.GetSceneNames  	= (std::vector<const char*> (*)()) GetProcAddress(g_EngineLibraryHandle,
		"_Z13GetSceneNamesv"
	);

	if (api.EngineInit != nullptr) {
		api.EngineInit();
	}

	// isEngineLoadSuccess(api);
	if (!api.is_loaded) {
		printf("WARNING: [win32] library `" ENGINE_LIB_NAME_OS_WINDOWS "` partial load.\n");
	} else {
		printf("INFO: [win32] library `" ENGINE_LIB_NAME_OS_WINDOWS "` loaded successfully.\n");
	}
}


} // end namespace win32


