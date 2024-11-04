#ifndef GFE_VECTOR_H
#define GFE_VECTOR_H

#include <type_traits>
#include <stdint.h>
#include <array>
#ifdef __linux
#include <math.h>
#endif

namespace GFE {

template <typename T> struct IntSel {};
template <> struct IntSel<double> {
    typedef int64_t type;
    static constexpr int64_t value = 0x5fe6eb50c7b537a9;
};
template <> struct IntSel<float> {
    typedef int32_t type;
    //static constexpr int32_t value = 0x5f3759df;
    static constexpr int32_t value = 0x5F375A86;
};

// 写成模板避免被其他类型隐式调用
template <typename T/*, typename = std::enable_if_v<std::is_floating_point_v<T>>*/>
T FastInvSqrt(T v){
    T x2 = v * T(0.5);
    typename IntSel<T>::type i = *(typename IntSel<T>::type*)&v;
    i = IntSel<T>::value - (i >> 1);
    v = *(T*) &i;
    v = v * (T(1.5) - (x2 * v * v));   		// 1st iteration
    v = v * (T(1.5) - (x2 * v * v));		// 2nd iteration
    return v;
}

class Vec3D {
public:

    Vec3D(){}
    Vec3D(double _x, double _y, double _z) :
        x(_x), y(_y), z(_z) {}
    Vec3D(const double* raw) :
    	x(raw[0]), y(raw[1]), z(raw[2]) {}
    Vec3D(const std::array<double, 3>& raw) :
    	x(raw[0]), y(raw[1]), z(raw[2]) {}
    Vec3D(const std::array<double, 3>& begin, const std::array<double, 3>& end) :
          x(end[0]-begin[0]), y(end[1]-begin[1]),z(end[2]-begin[2]) {}
    Vec3D(const double* begin, const double* end) :
          x(end[0]-begin[0]), y(end[1]-begin[1]),z(end[2]-begin[2]) {}

    std::array<double, 3> ToArray() const {
    	return {x, y, z};
    }

    Vec3D operator -() const {
        return Vec3D{-x,-y,-z};
    }

    Vec3D& operator+= (const Vec3D& rhs) {
    	x += rhs.x;
    	y += rhs.y;
    	z += rhs.z;
    	return *this;
    }

    Vec3D& operator-= (const Vec3D& rhs) {
    	x -= rhs.x;
    	y -= rhs.y;
    	z -= rhs.z;
    	return *this;
    }

    Vec3D& operator*= (const double factor) {
    	x *= factor;
    	y *= factor;
    	z *= factor;
    	return *this;
    }

    friend Vec3D operator+ (const Vec3D& v1, const Vec3D& v2) {
        return Vec3D(v1) += v2;
    }
    friend Vec3D operator- (const Vec3D& v1, const Vec3D& v2) {
        return Vec3D(v1) -= v2;
    }
    friend Vec3D operator* (const Vec3D& v1, const double factor) {
    	return Vec3D(v1) *= factor;
    }
    friend Vec3D operator* (const double factor, const Vec3D& v1) {
    	return Vec3D(v1) *= factor;
    }

    Vec3D scale(const double v) const {
    	return Vec3D{x*v, y*v, z*v};
    }
    // 内积
    static double dot(const Vec3D& v1, const Vec3D& v2) {
        return v1.x*v2.x+v1.y*v2.y+v1.z*v2.z;
    }
    double dot(const Vec3D& v) const {
        return x*v.x + y*v.y + z*v.z;
    }
    // 外积
    static Vec3D cross(const Vec3D& v1, const Vec3D& v2) {
        return {v1.y*v2.z-v2.y*v1.z, v1.z*v2.x-v2.z*v1.x, v1.x*v2.y-v2.x*v1.y};
    }
    Vec3D cross(const Vec3D& v) const {
        return {y*v.z-v.y*z, z*v.x-v.z*x, x*v.y-v.x*y};
    }
    static double mold(const Vec3D& v1) {
        return sqrt(dot(v1,v1));
    }
    double mold() const {
        return sqrt(dot(*this));
    }
    static double cos(const Vec3D& v1, const Vec3D& v2) {
        return dot(v1, v2)/mold(v1)/mold(v2);
    }
    double cos(const Vec3D& v) const {
        return dot(v)/this->mold()/v.mold();
    }
    static Vec3D normalize(const Vec3D& v){
        double vv = FastInvSqrt(v.x*v.x + v.y*v.y + v.z*v.z);
        return Vec3D{v.x*vv, v.y*vv, v.z*vv};
    }
    Vec3D normalize() {
        auto& v = *this;
        double vv = FastInvSqrt(v.x*v.x + v.y*v.y + v.z*v.z);
        return v.scale(vv);
    }

    double x=0,y=0,z=0;

};

class LineSeg {
public:
    LineSeg() = default;
    LineSeg(const std::array<double, 3>& pt1, const std::array<double, 3>& pt2) : _pt1(pt1), _pt2(pt2) {};
    LineSeg(const LineSeg& rhs) : _pt1(rhs._pt1), _pt2(rhs._pt2) {}
    LineSeg(LineSeg&& rhs) : _pt1(std::move(rhs._pt1)), _pt2(std::move(rhs._pt2)) {}
    LineSeg& operator =(const LineSeg&);
    LineSeg& operator =(LineSeg&&);
    const auto& Pt1() const { return _pt1; }
    const auto& Pt2() const { return _pt2; }
    double Length() const {
        auto& [x1,y1,z1] = _pt1;
        auto& [x2,y2,z2] = _pt2;
        return sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)+(z2-z1)*(z2-z1));
    }
    Vec3D ToVec() const {
        auto& [x1,y1,z1] = _pt1;
        auto& [x2,y2,z2] = _pt2;
        return Vec3D(x2-x1,y2-y1,z2-z1);
    }
    //! 是否相交
    //! @return 0 不相交 1 共线相交 2 不共线相交
    int IsIntersect(const LineSeg& other
                    , double tol1 = 1e-10, double tol2 = 1e-8) { return IsIntersect(*this, other, tol1, tol2); }
    static int IsIntersect(const LineSeg& ls1, const LineSeg& ls2
                           , double tol1 = 1e-10, double tol2 = 1e-8);
    //! 判断点是否在线段上
    bool IsInside(const std::array<double, 3>& pt, double tol = 1e-8) { return IsInside(*this, pt, tol); }
    static bool IsInside(const LineSeg& ls, const std::array<double, 3>& pt, double tol = 1e-8);

    //! 点到直线的距离
    static double Height(const std::array<double, 3>& pt, const LineSeg& ls) {
        return Vec3D::cross(Vec3D(ls.Pt1(), pt), Vec3D(ls.Pt1(), ls.Pt2())).mold()/ls.Length();
    }

    //! 点到线段的距离（未实现)
//    double Distance(const std::array<double, 3>& pt);

private:
    std::array<double, 3> _pt1, _pt2;
};

}

#endif // GFE_VECTOR_H
