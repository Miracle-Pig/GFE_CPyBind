#pragma once
#include <cmath>

namespace GFE {

/**
 * 插值函数（线性）
 */
template <class T>
class Interp
{
    T* x = nullptr;
    T* y = nullptr;
    int n = 0;
    T upper = T(NAN);
    T lower = T(NAN);
public:
    enum {
        TABULAR,

        // 未实现
        TABULAR_XLOG,
        TABULAR_YLOG,
        TABULAR_XYLOG,

        // 已实现
        SMOOTH_STEP,
        QUADRATIC,

        // 未实现
        POWER_SERIES,
    };

    bool extrapolate = false;       // 外插
    int type = TABULAR;

    Interp() {}
    Interp(const T* _x, const T* _y, unsigned long long _n) {
        Init(_x, _y, _n);
    }
    Interp(const T* _xy, unsigned long long _n) {
        Init(_xy, _n);
    }
    Interp(const Interp& rhs) {
        *this = rhs;
    }
    Interp& operator=(const Interp& rhs) {
        this->Init(rhs.x, rhs.y, rhs.n);
        return *this;
    }
    Interp(Interp&& rhs) {
        *this = rhs;
    }
    Interp& operator=(Interp&& rhs)
    {
        this->x = rhs.x;
        this->y = rhs.y;
        this->n = rhs.n;
        this->lower = rhs.lower;
        this->upper = rhs.upper;
        rhs.x = nullptr;
        rhs.y = nullptr;
        return *this;
    }
    virtual ~Interp() {
        if(x) delete[] x;
        if(y) delete[] y;
        x = nullptr;
        y = nullptr;
    }
    void Init(const T* _xy, unsigned long long _n)
    {
        if(!_xy) return;
        if(_n <= 0) return;
        if(x) delete[] x;
        if(y) delete[] y;
        n = _n/2;
        x = new T[n];
        y = new T[n];
        for(unsigned long long i = 0; i < n; i++) {
            x[i] = _xy[2*i];
            y[i] = _xy[2*i+1];
        }
        SetBound(y[0], y[n-1]);
    }
    void Init(const T* _x, const T* _y, unsigned long long _n)
    {
        if(!_x || !_y) return;
        if(_n <= 0) return;
        if(x) delete[] x;
        if(y) delete[] y;
        x = new T[_n];
        y = new T[_n];
        n = _n;
        for(unsigned long long i = 0; i < n; i++) {
            x[i] = _x[i];
            y[i] = _y[i];
        }
        SetBound(y[0], y[n-1]);
    }
    void InitRef(T* _x, T* _y, unsigned long long _n)
    {
        if(!_x || !_y || _n <= 0) return;
        this->x = _x;
        this->y = _y;
        this->n = _n;
        SetBound(y[0], y[n-1]);
    }
    void Clear() {
        if(x) delete[] x;
        x = nullptr;
        if(y) delete[] y;
        y = nullptr;
        n = 0;
        upper = T(NAN);
        lower = T(NAN);
    }
    void SetBound(T _lower, T _upper) {
        lower = _lower;
        upper = _upper;
    }
    void GetBound(T& _lower, T& _upper) {
        _lower = lower;
        _upper = upper;
    }
    bool IsEmpty() {
        return n==0;
    }
    virtual T Value(const T& xIn) const
    {
        if (type == TABULAR)
            return Value_Linear(xIn);
        if (type == SMOOTH_STEP)
            return Value_Smooth(xIn);
        if (type == QUADRATIC)
            return Value_Quadratic(xIn);
        return 0;
    }

    virtual T Value_Linear(const T& xIn) const;
    virtual T Value_Smooth(const T& xIn) const;
    virtual T Value_Quadratic(const T& xIn) const;
};

template class Interp<double>;

template<class T>
T Interp<T>::Value_Linear(const T& xIn) const
{
    constexpr T err = T(1e-10);
    if(n <= 0) return NAN;
    if(!extrapolate && xIn-x[n-1]>err) return upper;
    if(!extrapolate && x[0]-xIn>err) return lower;
    for(int i = 1; i < n; i++)
    {
        if(xIn<x[i])
        {
            auto k = (y[i]-y[i-1])/(x[i]-x[i-1]);
            return y[i-1]+k*(xIn-x[i-1]);
        }
    }
    if(extrapolate) {
        auto k = (y[n-1]-y[n-2])/(x[n-1]-x[n-2]);
        return y[n-1]+k*(xIn-x[n-1]);
    }
    else
        return y[n-1];
}

template<class T>
T Interp<T>::Value_Smooth(const T& xIn) const
{
    if(n <= 0)
        return NAN;
    if(x[0] > xIn)
        return y[0];
    if(x[n-1] < xIn)
        return y[n-1];
    // find t0

    T t0, t1, A0, A1;
    for(int i = 0; i < n; i++)
        if(x[i] <= xIn) {
            t0 = x[i];
            t1 = x[i+1];
            A0 = y[i];
            A1 = y[i+1];
        }


    T ks = (xIn-t0)/(t1-t0);

    return A0+(A1-A0)*std::pow(ks, 3)*(10-15*ks+6*ks*ks);
}

template <typename T>
T QuadraticInterpolate(T x0, T y0, T x1, T y1, T x2, T y2, T x) {
    // 使用拉格朗日插值公式进行二次插值
    T L0 = ((x - x1) * (x - x2)) / ((x0 - x1) * (x0 - x2));
    T L1 = ((x - x0) * (x - x2)) / ((x1 - x0) * (x1 - x2));
    T L2 = ((x - x0) * (x - x1)) / ((x2 - x0) * (x2 - x1));

    return y0 * L0 + y1 * L1 + y2 * L2;
}

template<class T>
T Interp<T>::Value_Quadratic(const T& xIn) const
{
    if (n < 3) {
        // 如果数据点不足三个，退化成线性插值
        return Value_Linear(xIn);
    }

    // 确定大区间的划分
    for (int i = 1; i < n - 1; i += 2) {
        // 每个大区间由三个连续的点组成：x[i-1], x[i], x[i+1]
        if (xIn >= x[i - 1] && xIn <= x[i + 1]) {
            // 在当前大区间内执行二次插值
            return QuadraticInterpolate(x[i - 1], y[i - 1], x[i], y[i], x[i + 1], y[i + 1], xIn);
        }
    }

    // 如果xIn不在任何大区间内，可以选择外推、返回边界值或NAN
    if (!extrapolate) {
        if (xIn < x[0]) {
            return lower; // 或者y[0]
        } else {
            return upper; // 或者y[n-1]
        }
    }
    else {
        if (xIn < x[0]) {
            return QuadraticInterpolate(x[0], y[0], x[1], y[1], x[2], y[2], xIn);
        } else {
            return QuadraticInterpolate(x[n - 3], y[n - 3], x[n - 2], y[n - 2], x[n - 1], y[n - 1], xIn);
        }
    }

    //! @test 另一种方案，已弃用

    // // 首先，找到xIn所在的区间[x[i-1], x[i]]，或者最接近的区间
    // int i = 0;
    // while (i < n - 1 && xIn > x[i + 1]) {
    //     i++;
    // }

    // // 选择三个点进行二次插值
    // // 如果xIn小于第一个数据点或大于最后一个数据点，需要特殊处理
    // if (i == 0) {
    //     // 如果xIn小于第一个数据点，使用第一个、第二个和第三个数据点进行插值
    //     if (n < 3) {
    //         // 如果数据点不足三个，无法进行二次插值，这里可以选择线性插值或返回错误
    //         return Value_Linear(xIn);
    //     }
    //     return QuadraticInterpolate(x[0], y[0], x[1], y[1], x[2], y[2], xIn);
    // } else if (i == n - 1) {
    //     // 如果xIn大于最后一个数据点，使用倒数第三个、倒数第二个和最后一个数据点进行插值
    //     if (n < 3) {
    //         // 如果数据点不足三个，无法进行二次插值，这里可以选择线性插值或返回错误
    //         return Value_Linear(xIn);
    //     }
    //     return QuadraticInterpolate(x[n - 3], y[n - 3], x[n - 2], y[n - 2], x[n - 1], y[n - 1], xIn);
    // } else {
    //     // 在中间区域，使用x[i-1], x[i], x[i+1]三个点进行插值
    //     return QuadraticInterpolate(x[i - 1], y[i - 1], x[i], y[i], x[i + 1], y[i + 1], xIn);
    // }
}

}
