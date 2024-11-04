#ifndef GFE_HEX_H
#define GFE_HEX_H

#include "GFE_Array.h"

namespace GFE {
class Hex {
public:
	//! 转发到GFE::Array构造
    template <typename... Args>
    Hex(Args&&... values) : pt_(std::forward<Args&&>(values)...) {}

	//! 体积
    double GetVolume() const;

    //! 暂未实现
    double GetJacobian() const;


private:
	GFE::Array2<double, 8, 3> pt_;

};
}


#endif // GFE_HEX_H
