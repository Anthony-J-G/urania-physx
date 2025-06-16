#include "win32_dynamic_api.hpp"

#include <stdio.h>
#include <libloaderapi.h>
#include <windows.h>

namespace win32 {

HMODULE g_PhysicsLibraryHandle;               // Handle to DLL


void LoadEngineLibrary(PhysicsLibrary& api) {
	g_PhysicsLibraryHandle = LoadLibrary("physics.dll");
	if (g_PhysicsLibraryHandle == NULL) {
		printf("[win32] Error: library `physics.dll` could not be loaded\n");
		return;
	}

	api.is_loaded = true; // Assume true, and set false if a procedure fails to load

	api.EngineInit 	= (void (*)()) GetProcAddress(g_PhysicsLibraryHandle,
		"_Z10InitEnginev"
	);
	api.GetScene  	= (Scene_API* (*)(const char*)) GetProcAddress(g_PhysicsLibraryHandle,
		"GetScene"
	);
	api.GetSceneNames  	= (std::vector<const char*> (*)()) GetProcAddress(g_PhysicsLibraryHandle,
		"_Z13GetSceneNamesv"
	);

	if (api.EngineInit != nullptr) {
		api.EngineInit();
	}
}





} // end namespace win32


