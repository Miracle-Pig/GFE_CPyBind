#pragma once

#ifdef _MSC_VER
#if defined(GFE_Solver_EXPORT)
#define GFE_Solver_DLIB __declspec(dllexport)
#else
#define GFE_Solver_DLIB __declspec(dllimport)
#endif
#elif defined __GNUC__
#define GFE_Solver_DLIB __attribute__((__visibility__("default")))
#endif
