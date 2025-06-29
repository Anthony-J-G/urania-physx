#pragma once



#if defined (_WIN32) || defined (_WIN64)
#   ifdef ENGINE_LIBRARY_EXPORTS
#       define ENGINE_API __declspec(dllexport)
#   else
#       define ENGINE_API __declspec(dllimport)
#   endif // ENGINE_LIBRARY_EXPORTS
#else
#   define ENGINE_API
#endif