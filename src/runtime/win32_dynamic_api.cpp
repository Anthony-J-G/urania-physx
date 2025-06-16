#include "win32_dynamic_api.hpp"

#include <stdio.h>
#include <libloaderapi.h>
#include <windows.h>

namespace win32 {

HMODULE g_PhysicsLibraryHandle;               // Handle to DLL
// LPFNDLLFUNC1 lpfnDllFunc1;    // Function pointer
DWORD dwParam1;
UINT  uParam2, uReturnVal;


void LoadDynamicLibrary(PhysicsLibrary& api) {
	g_PhysicsLibraryHandle = LoadLibrary("physics.dll");
	if (g_PhysicsLibraryHandle == NULL) {
		printf("[win32] Error: library `physics.dll` could not be loaded\n");
		return;
	}

	api.is_loaded = true; // Assume true, and set false if a procedure fails to load

	int x;
	api.EngineInit 	= (void (*)()) GetProcAddress(g_PhysicsLibraryHandle,
		"_Z10InitEnginev"
	);
	api.GetScene  	= (Scene* (*)(const char*)) GetProcAddress(g_PhysicsLibraryHandle,
		"GetScene"
	);
	api.Scene__Init = (void (*)()) GetProcAddress(g_PhysicsLibraryHandle,
		"_ZN22OscillatingCircleScene4InitEv"
	);
	api.Scene__Update = (void (*)(float)) GetProcAddress(g_PhysicsLibraryHandle,
		"_ZN22OscillatingCircleScene6UpdateEf"
	);
	api.Scene__Render = (void (*)()) GetProcAddress(g_PhysicsLibraryHandle,
		"_ZN22OscillatingCircleScene6RenderEv"
	);
	api.Scene__Shutdown = (void (*)()) GetProcAddress(g_PhysicsLibraryHandle,
		"_ZN22OscillatingCircleScene8ShutdownEv"
	);
	
	api.is_loaded = api.is_loaded && 
		(api.EngineInit != nullptr) &&
		(api.Scene__Init != nullptr) &&
		(api.Scene__Update != nullptr)
	;
}





} // end namespace win32


