#include "win32_dynamic_api.hpp"

#include <stdio.h>
#include <libloaderapi.h>
#include <windows.h>

namespace win32 {

HINSTANCE g_PhysicsLibraryHandle;               // Handle to DLL
// LPFNDLLFUNC1 lpfnDllFunc1;    // Function pointer
DWORD dwParam1;
UINT  uParam2, uReturnVal;


void LoadDynamicLibrary(PhysicsLibrary& api) {
	g_PhysicsLibraryHandle = LoadLibrary("physics.dll");
	if (g_PhysicsLibraryHandle == NULL) {
		printf("[win32] Error: library `physics.dll` could not be loaded\n");
	}
}





} // end namespace win32


