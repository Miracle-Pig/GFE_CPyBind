#ifndef GFE_WEDGE_H
#define GFE_WEDGE_H

#include "GFE_Array.h"

namespace GFE {
class Wedge {
public:
	//! 转发到GFE::Array构造
    template <typename... Args>
    Wedge(Args&&... values) : pt_(std::forward<Args&&>(values)...) {}

	//! 体积
    double GetVolume() const;

    //! 暂未实现
    double GetJacobian() const;


private:
	GFE::Array<GFE::Array<double, 3>, 6> pt_;

};
}

#endif // GFE_WEDGE_H
