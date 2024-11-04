#pragma once
#include "Math/GFE_Vector.h"

namespace GFE {
class Triangle {
public:
    Triangle() = default;
    Triangle(const std::array<std::array<double, 3>,3>& pt);
    Triangle(const Triangle& rhs) : _pt(rhs._pt), _ls(rhs._ls) {}
    Triangle(Triangle&& rhs) : _pt(std::move(rhs._pt)), _ls(std::move(rhs._ls)) {}
    Triangle& operator =(const Triangle&);
    Triangle& operator =(Triangle&&);

    //! 纵横比, 越小网格质量越好
    //! Top: 20, Upper threshold: 5, Lower threshold: 3, Bottom: 1
    double GetAspectRatio() const;

    //! 偏斜
    //! Top: 48, Upper threshold: 36, Lower threshold: 18, Bottom: 0
    //! pi有小误差，导致结果有小偏差 （出现较小负数属于正常情况）
    //! 返回相对偏斜
    double GetSkew() const;

    //! Jacobian Rate
    double GetJacobianRate() const;

    //! 面积
    double GetArea() const;

    //! 最小内角（角度制）
    //! @param id 取得最小角度的角id
    double GetMinAngle(int* id = nullptr) const;

private:
    std::array<std::array<double, 3>,3> _pt;
    std::array<LineSeg, 3> _ls;
};
}

