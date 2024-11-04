#ifndef GFE_TETUTIL_H
#define GFE_TETUTIL_H

#include <GFE_Array.h>

namespace GFE {
class Tet {
public:
	//! 转发到GFE::Array构造
    template <typename... Args>
    Tet(Args&&... values) : pt_(std::forward<Args&&>(values)...) {}

	//! 体积
    double GetVolume() const;

    //! 暂未实现
    double GetJacobian() const;


private:
	GFE::Array2<double, 4, 3> pt_;

	//! 二面角
	//static std::pair<double, double> getMinMaxAngle(const std::array<Vec3D, 4>& coordinate);

};

class Tet2 {
public:
	//! 转发到GFE::Array构造
    template <typename... Args>
    Tet2(Args&&... values) : pt_(std::forward<Args&&>(values)...) {}

	//! 暂未实现
    double GetVolume() const;

    //! 暂未实现
    double GetJacobian() const;

private:
	GFE::Array2<double, 10, 3> pt_;

};
}




#endif // GFE_TETUTIL_H
