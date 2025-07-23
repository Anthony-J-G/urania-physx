// ** `examples` includes
#include "examples/app.h"

// ** `stdlib` includes
#include <stdlib.h>


#if defined(_WIN32)
	#include <crtdbg.h>

	#ifndef WIN32_LEAN_AND_MEAN
		#define WIN32_LEAN_AND_MEAN
	#endif

	#include <windows.h>
#endif


int main(int argc, char* argv[]) {
	
#if defined(_WIN32)
	// TODO(anthony-j-g): link against win ucrt to allow for memory-leak reporting
	// Enable memory-leak reports
	// _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG | _CRTDBG_MODE_FILE);
	// _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
#endif

	Initialize();

	// Main game loop
	while (!ShouldClose()) { // Detect window close button or ESC key		
		Update();
		Draw();
	}

	Destroy();

	return EXIT_SUCCESS;
}
