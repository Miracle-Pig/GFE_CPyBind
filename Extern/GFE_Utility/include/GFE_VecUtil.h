#pragma once
#include <vector>
#include <set>
#include <iostream>
#ifdef __linux
#include <experimental/memory_resource>
namespace std {
namespace pmr {
template <class _Ty>
using vector = ::std::vector<_Ty, experimental::pmr::polymorphic_allocator<_Ty>>;
}
}

#endif

namespace GFE
{
template<class Container1, class Container2>
Container1 TakeMulti(Container1& beToken, const Container2& takeId)
{
    Container1 take;
    if(beToken.empty() || takeId.begin() == takeId.end())
        return take;

    std::size_t cc = 0;
    auto n = beToken.size();
    auto takeIdItr = takeId.begin();
    for(std::size_t i = 0; i < n; i++)
    {
        if(cc > 0)
            beToken[i-cc] = beToken[i];		// move
        if(*takeIdItr == i) {
            take.push_back(beToken.at(i));		// move
            cc++;
            takeIdItr++;
        }
    }
    beToken.erase(beToken.end()-cc, beToken.end());
    return take;
}

namespace {
// 命名规则参照STL, 类和成员函数小写

template<typename... _Items>
class vector_items;

template <>
class vector_items<> {
public:
	void clear() {}
};

template <std::size_t Index, typename _Vector>
struct vector_index_helper;

template <typename _This, typename... _Other>
struct vector_index_helper<0, vector_items<_This, _Other...>> {
	typedef std::vector<_This> element_type;
	typedef vector_items<_This, _Other...> vector_type;
};

template <std::size_t Index, typename _This, typename... _Other>
struct vector_index_helper<Index, vector_items<_This, _Other...>>
	: public vector_index_helper<Index - 1, vector_items<_Other...>> {};

template <typename _type, typename... _Args>
constexpr bool hasType() {
	return (false || ... || std::is_same_v<_type, _Args>);
}

template <typename _Tp, typename _Vector>
struct vector_type_helper;

template <typename _This, typename... _Other>
struct vector_type_helper<_This, vector_items<_This, _Other...>> {
    static_assert (!hasType<_This, _Other...>(), "vecotr_items get ambigious type!");
	typedef std::vector<_This> element_type;
	typedef vector_items<_This, _Other...> vector_type;
};

template <typename _tp, typename _This, typename... _Other>
struct vector_type_helper<_tp, vector_items<_This, _Other...>>
	: public vector_type_helper<_tp, vector_items<_Other...>> {};


template <typename _This, typename... _Other>
class vector_items<_This, _Other...> : public vector_items<_Other...>
{
public:
	void clear() {
		value.clear();
		vector_items<_Other...>::clear();
	}
	std::vector<_This> value;
};

template <std::size_t Index, typename... _Args>
constexpr const typename vector_index_helper<Index, vector_items<_Args...>>::element_type&
Get(const vector_items<_Args...>& vector) {
	return static_cast<typename vector_index_helper<Index, vector_items<_Args...>>::vector_type const&>(vector).value;
}

template <std::size_t Index, typename... _Args>
constexpr typename vector_index_helper<Index, vector_items<_Args...>>::element_type&
Get(vector_items<_Args...>& vector) {
	return static_cast<typename vector_index_helper<Index, vector_items<_Args...>>::vector_type&>(vector).value;
}

template <typename _Tp, typename... _Args>
constexpr const typename vector_type_helper<_Tp, vector_items<_Args...>>::element_type&
Get(const vector_items<_Args...>& vector) {
	return static_cast<typename vector_type_helper<_Tp, vector_items<_Args...>>::vector_type const&>(vector).value;
}

template <typename _Tp, typename... _Args>
constexpr typename vector_type_helper<_Tp, vector_items<_Args...>>::element_type&
Get(vector_items<_Args...>& vector) {
	return static_cast<typename vector_type_helper<_Tp, vector_items<_Args...>>::vector_type&>(vector).value;
}

}

// Wrapper
template <typename... Args>
class VectorItems {
public:
    template <typename _Tp>
	static constexpr bool has_item() {
		return hasType<_Tp, Args...>();
	}
	static constexpr std::size_t size() {
		return sizeof... (Args);
	}
public:
	VectorItems() = default;
	/*
	VectorItems(const VectorItems& rhs) noexcept {
		// if possible, don't copy
		std::cout << "Developer warning: copy VectorItems." << std::endl;
		rhs.apply([this](const auto& v) {
            this->get<typename std::remove_reference_t<decltype(v)>::value_type>() = v;
		});
		rhs.apply([this](const auto& v) {
            std::cout << typeid (typename std::remove_reference_t<decltype (v)>::value_type).name() << std::endl;
		});
	}
	*/
	VectorItems(const VectorItems& rhs) noexcept = delete;

	VectorItems(VectorItems&& rhs) noexcept {
		rhs.apply([this](auto&& v) {
            this->get<typename std::remove_reference_t<decltype(v)>::value_type>() = std::move(v);
		});
	}
	/*
	VectorItems& operator= (const VectorItems& rhs) noexcept {
		// if possible, don't copy
		std::cout << "Developer warning: copy VectorItems." << std::endl;
		rhs.apply([this](const auto& v) {
            this->get<typename std::remove_reference_t<decltype(v)>::value_type>() = v;
		});
		return *this;
	}
	*/
	VectorItems& operator= (const VectorItems& rhs) noexcept = delete;

	VectorItems& operator= (VectorItems&& rhs) noexcept {
		rhs.apply([this](auto&& v) {
            this->get<typename std::remove_reference_t<decltype(v)>::value_type>() = std::move(v);
		});
		return *this;
	}

public:
	//! clear all
	void clear() { data_.clear(); }
	template <typename _Tp>
	void clear() { Get<_Tp>(data_).clear(); }

    //template <typename _Tp>
	//typename std::enable_if_t<!HasItem<_Tp>(), std::vector<_Tp>> Data() const { return {}; }

    template<typename _Tp>
	constexpr const auto& get() const {
        static_assert (has_item<_Tp>(), "VectorItems invalid type");
		return Get<_Tp>(data_);
	}

    template<typename _Tp>
	constexpr auto& get() {
        static_assert (has_item<_Tp>(), "VectorItems invalid type");
		return Get<_Tp>(data_);
	}

	template <std::size_t Index>
    constexpr const auto& get() const {
        static_assert (Index < size(), "VectorItems index out of range!");
        return Get<Index>(data_);
    }

	template <std::size_t Index>
    constexpr auto& get() {
        static_assert (Index < size(), "VectorItems index out of range!");
        return Get<Index>(data_);
    }

    template <std::size_t Index, typename _Tp>
    void push_back(_Tp&& value) {
    	get<Index>().push_back(std::forward<_Tp>(value));
    }

    template<typename _Tp>
    std::enable_if_t<has_item<std::remove_reference_t<_Tp>>(), void>
    push_back(_Tp&& value) {
    	get<std::remove_reference_t<_Tp>>().push_back(std::forward<_Tp>(value));
    }

    template <std::size_t Index, typename... _Args>
    auto& emplace_back(_Args&&... values) {
    	return get<Index>().emplace_back(std::forward<_Args>(values)...);
    }

    template<typename _Tp, typename... _Args>
    std::enable_if_t<has_item<std::remove_reference_t<_Tp>>(), _Tp&> emplace_back(_Args&&... values) {
    	return get<_Tp>().emplace_back(std::forward<_Args>(values)...);
    }

	template <typename Func>
	//! apply function to all vectors, equally call Func(get<0>()), Func(get<1>()), ...,
	void apply(Func&& function) {
		apply_proxy(std::forward<Func>(function), std::make_index_sequence<size()>());
	}
	template <typename Func>
	//! apply function to all vectors, equally call Func(get<0>()), Func(get<1>()), ...,
	void apply(Func&& function) const {
		apply_proxy(std::forward<Func>(function), std::make_index_sequence<size()>());
	}

	// 更多函数，暂未实现

private:
	vector_items<Args...> data_;

private:
	template <typename Func, std::size_t... Index>
	void apply_proxy(Func&& f, std::integer_sequence<std::size_t, Index...> seq) {
		(f(get<Index>()),...);
	}
	template <typename Func, std::size_t... Index>
	void apply_proxy(Func&& f, std::integer_sequence<std::size_t, Index...> seq) const {
		(f(get<Index>()),...);
	}

};

}


