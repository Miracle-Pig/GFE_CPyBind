#ifndef GFE_ARRAY_H
#define GFE_ARRAY_H

#include <memory>
#include <utility>
#include <variant>
#include <array>
#include <stdexcept>
#include <unordered_set>
#include <unordered_map>
#ifdef __linux
#include <algorithm>
#include <vector>
#endif

namespace GFE {

//! 数组，本质上是std::array和裸指针的union
template <typename T, std::size_t N>
class Array {
public:
	Array() : v_(nullptr) {}
	//! initializer list
	Array(std::initializer_list<T> list) : v_(std::array<T, N>{}) {
		//std::initializer_list::size方法为constexpr但这里并没有返回一个编译器常量
        //static_assert (N >= list.size(), "Error: try to construct GFE::Array from a larger size initializer list.");
        std::copy_n(list.begin(), std::min(N, list.size()), data());
	}
	Array(std::array<T, N>&& arr) : v_(std::move(arr)) {}
	Array(T* arr) : v_(arr) {}

	//! 构造函数，转发到std::variant构造
    //template <typename ...Args>
    //Array(Args&&... values) : v_(std::forward<Args&&>(values)...) {}

public:

	//! 获取底层数据指针
	constexpr const T* data() const {
		if (std::holds_alternative<T*>(v_))
			return std::get<1>(v_);
		else
			return std::get<0>(v_).data();
	}
    constexpr T* data() {
		return const_cast<T*>(static_cast<const Array*>(this)->data());
    }
	//! 下标访问
	constexpr const T& operator[] (std::size_t pos) const {
		return this->data()[pos];
	}
	constexpr T& operator[] (std::size_t pos) {
		return const_cast<T&>(static_cast<const Array*>(this)->operator[](pos));
	}
	//! at访问，带有边界检查
	constexpr const T& at(std::size_t pos) const {
		if (pos >= N)
            throw std::out_of_range("GFE::Array acess data out of range.");
        return this->operator[](pos);
	}


	// 更多函数，暂未实现

private:
	std::variant<std::array<T, N>, T*> v_;
};

//! 二维数组
template <typename T, std::size_t N1, std::size_t N2>
using Array2 = GFE::Array<GFE::Array<T, N2>, N1>;



}

#endif // GFE_ARRAY_H
