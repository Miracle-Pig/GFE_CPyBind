#pragma once
#include <GFE_Solver_Global.h>
#include <array>

namespace GFE {
using std::array;
//! @arg integ 长度为4的数组
//! @arg type 0 C3D10 1 C3D10M
array<float,10> C3D10_Extrap(const float* integ, int type = 0);
array<double,10> C3D10_Extrap(const double* integ, int type = 0);

}

