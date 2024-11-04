#pragma once
#include <type_traits>

namespace GFE {
// 浮点数累加, Kahan summation算法
template <typename _Tp>
typename std::enable_if<std::is_floating_point<_Tp>::value>::type
FloatAcc(_Tp* arr, std::size_t size) {
    double C = 0, Y, T;
    for(std::size_t i = 1; i < size; i++) {
        Y = arr[i] - C;
        T = arr[i-1] + Y;
        C = T - arr[i-1] - Y;
        arr[i] = T;
    }
}

//template <typename _Tp>
//typename std::enable_if<std::is_floating_point<_Tp>::value, _Tp>::type
//FloatAcc(const _Tp* arr, std::size_t size) {
//    _Tp S = 0, C = 0, Y, T;
//    for(std::size_t i = 0; i < size; i++) {
//        Y = arr[i] - C;
//        T = S + Y;
//        C = T - S - Y;
//        S = T;
//    }
//    return S;
//}

// 浮点数扫描（前缀求和）, Kahan summation算法
template <typename _Tp>
typename std::enable_if<std::is_floating_point<_Tp>::value>::type
FloatScan(_Tp* arr, std::size_t size) {
    _Tp C = 0, Y, T;
    for(std::size_t i = 1; i < size; i++) {
        Y = arr[i] - C;
        T = arr[i-1] + Y;
        C = T - arr[i-1] - Y;
        arr[i] = T;
    }
}
}
