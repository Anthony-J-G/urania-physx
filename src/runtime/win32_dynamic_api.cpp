#include <windows.h>


namespace win32 {

HINSTANCE hDLL;               // Handle to DLL
LPFNDLLFUNC1 lpfnDllFunc1;    // Function pointer
DWORD dwParam1;
UINT  uParam2, uReturnVal;


void LoadDynamicLibrary(PhysicsLibrary& api) {
	HINSTANCE hDLL;               // Handle to DLL
	LPFNDLLFUNC1 lpfnDllFunc1;    // Function pointer
	DWORD dwParam1;
	UINT  uParam2, uReturnVal;

	hDLL = LoadLibrary("MyDLL");
	if (hDLL != NULL) {
	   lpfnDllFunc1 = (LPFNDLLFUNC1)GetProcAddress(hDLL,
                                           "DLLFunc1");
	   if (!lpfnDllFunc1) {
	      // handle the error
	      FreeLibrary(hDLL);
	      return SOME_ERROR_CODE;
	   } else {
	      // call the function
	      uReturnVal = lpfnDllFunc1(dwParam1, uParam2);
	   }
	}
}





} // end namespace win32


