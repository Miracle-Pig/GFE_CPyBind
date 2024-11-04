#pragma once
#include "Math/GFE_Vector.h"

namespace GFE {
class Quad {
public:
    Quad() = default;
    Quad(const std::array<std::array<double, 3>,4>& pt);
    Quad(const Quad& rhs) : _pt(rhs._pt), _ls(rhs._ls) {}
    Quad(Quad&& rhs) : _pt(std::move(rhs._pt)), _ls(std::move(rhs._ls)) {}
    Quad& operator =(const Quad&);
    Quad& operator =(Quad&&);

    //! 判断四边形内角是否过大（过于扁平）, 如果是, 返回过大的角点id, 否则返回-1
    //! @param threshold 角度阈值
    int GetLargeAngle(double threshold = 160) const;

    //! 判断是否为凹四边形（包含角度为180°的情况）, 如果是, 返回凹角id, 否则返回-1
    //! @attention 需要确保四边形共面
    int IsConcave() const;

    //! 判断四边形是否共面
    bool IsCoplanar(double tol = 0.2) const;

    //! 纵横比, 越小网格质量越好
    //! Top: 20, Upper threshold: 5, Lower threshold: 3, Bottom: 1
    //! 适用于凸四边形
    double GetAspectRatio() const;

    //! 偏斜
    //! Top: 80, Upper threshold: 60, Lower threshold: 30, Bottom: 0
    //! pi有小误差，导致结果有小偏差 （出现较小负数属于正常情况）
    //! 返回相对偏斜
    double GetSkew() const;

    //! Jacobian Rate
    //! 适用于凸四边形
    double GetJacobianRate() const;

    //! 面积
    double GetArea() const;

    //! 最小内角（角度制）
    //! @param id 取得最小角度的角id
    double GetMinAngle(int* id = nullptr) const;

private:
    std::array<std::array<double, 3>,4> _pt;
    std::array<LineSeg, 4> _ls;
};
}

