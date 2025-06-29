#pragma once


#include "common_api.hpp"

#define ENGINE_LIB_NAME_OS_WINDOWS ENGINE_LIB_NAME ".dll"

namespace win32 {

void LoadEngineLibrary(EngineLibrary& api);

}
