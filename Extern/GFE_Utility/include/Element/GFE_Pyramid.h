#ifndef GFE_PYRAMID_H
#define GFE_PYRAMID_H

#include "GFE_Array.h"

namespace GFE {
class Pyramid {
public:
	//! 转发到GFE::Array构造
    template <typename... Args>
    Pyramid(Args&&... values) : pt_(std::forward<Args&&>(values)...) {}

	//! 体积
    double GetVolume() const;

    //! 暂未实现
    double GetJacobian() const;


private:
	GFE::Array2<double, 5, 3> pt_;

};
}

#endif // GFE_PYRAMID_H
