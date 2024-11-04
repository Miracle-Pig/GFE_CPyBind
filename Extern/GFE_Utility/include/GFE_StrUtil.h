#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <type_traits>
#include <chrono>
#include <ctime>
#include <set>
#include <complex>
#ifdef _WIN32
#include <charconv>
#endif
#include <regex>

// 字符串跨平台处理（支持中文）
// Windows下使用wstring
// Unix下直接使用string(认为Unix下都是用utf-8编码)
#if _WIN32
#define CPStr(str) GFE::ToWString(str)
#else
#define CPStr(str) str
#endif

namespace GFE
{
    //! @brief Split 根据子字符串分割
    //! @attention 若noEmpty = false，即便传入的是空字符串，返回值中也会包含一个元素（空字符串）
    //! @attention 2024.7.8更新，所有Split函数内部都会对拆分后的子串自动进行Trim操作

    std::vector<std::string> Split(const std::string& in, const std::string& delim, bool noEmpty = false);
    std::set<std::string> Split2(const std::string& in, const std::string& delim, bool noEmpty = false);

    //! @brief 根据子字符串进行有限次分割
    //! @param time: 分割次数，返回值的size最大会是time + 1
    std::vector<std::string> SplitFin(const std::string& in, const std::string& delim, std::size_t time, bool noEmpty = false);

    //! @brief 根据一组分隔符(单字节)分割
    std::vector<std::string> Split(const std::string& in, const std::initializer_list<char>& delim, bool noEmpty = false);

    //! @brief 去除字符串左右两边的空格
    std::string Trim(const std::string& in, char toTrim = ' ');

    std::string ToLower(const std::string& in);
    std::string ToUpper(const std::string& in);
    bool IsNum(const char* str, int len = -1);
    inline bool IsReal(const char* str, int len = -1) { return IsNum(str, len); }
    bool IsComplex(const char* str, int len = -1);
    std::string ToString(const std::wstring&);
    std::wstring ToWString(const std::string&);

    template <template <typename, typename...> typename Container, typename... Args>
    std::string Join(const Container<std::string, Args...>& in, const std::string& seperator) {
        std::string R;
        for(const auto& str : in)
            R.append(str+seperator);
        if(!R.empty())
            R.pop_back();
        return R;
    }

    //! @brief 字符串每个空格左右两边加上双引号
    std::wstring QuoteSpace(const std::wstring&);
#ifndef _WIN32
	std::string QuoteSpace(const std::string& in);
#endif

#if _HAS_CXX17 || __cplusplus >= 201703L
    std::vector<std::string_view> Split(const std::string_view& in, const std::string& delim, bool noEmpty = false);
    std::vector<std::string_view> SplitFin(const std::string_view& in, const std::string& delim, std::size_t time, bool noEmpty = false);
    std::vector<std::string_view> Split(const std::string_view& in, const std::initializer_list<char>& delim, bool noEmpty = false);
    std::string_view Trim(const std::string_view& in, bool left = true, bool right = true);
    std::string ToLower(const std::string_view& in);
    std::string ToUpper(const std::string_view& in);
#endif

#if defined _HAS_CXX17
    template<typename S, typename _Tp>
    typename std::enable_if<std::is_arithmetic<typename std::remove_cv<_Tp>::type>::value, bool>::type
    FromString(const S& _str, _Tp& to) {
        static_assert (std::is_same<S, std::string>::value || std::is_same<S, std::string_view>::value, "1st arg only support string or string_view");
        auto str = Trim(_str);
        if(str.empty())
            return false;
        auto begin = str.data();
        if(str.front() == '+' && str.size() > 1)
            begin++;
        _Tp tmp;
        auto res = std::from_chars(begin, str.data()+str.size(),tmp);
        if(res.ptr == str.data()+str.size()) {
            to = tmp;
            return true;
        }
        return false;
    }

    template <typename S, typename _Tp>
    bool FromString(const S& str, _Tp& real, _Tp& img)
    {
        static_assert (std::is_same<S, std::string>::value || std::is_same<S, std::string_view>::value, "1st arg only support string or string_view");
        if(!IsComplex(str.data()))
            return false;

        std::regex pattern("[+-]?(\\d+\\.?\\d*|\\.\\d+)([Ee][+-]?\\d+)?i?|[+-]?i");
        std::cmatch result;
        auto start = std::is_same<S, std::string>::value ? str.c_str() : str.data();
        auto end = std::is_same<S, std::string>::value ? str.c_str()+str.size() : str.data()+str.size();

        auto ConvImg = [&img](const std::string_view& view)
        {
            if(view.empty()) {
                img = 0;
                return;
            }
            auto num = view.substr(0,view.size()-1);
            if(num.empty() || num == "+")
                img = 1;
            else if(num == "-")
                img = -1;
            else
                FromString(view.substr(0,view.size()-1), img);
        };

        // number 1
        std::regex_search(start, end, result, pattern);
        std::string_view view(result[0].first, result[0].length());
        if(view.back() == 'i') {
            real = 0;
            ConvImg(view);
        }
        else {
            // number 1
            FromString(view, real);
            // number 2
            start = result[0].second;
            std::regex_search(start, end, result, pattern);
            view = std::string_view(result[0].first, result[0].length());
            ConvImg(view);
        }
        return true;
    }
#else
    template<typename S>
    bool FromString(const S& _str, int& to) {
        static_assert (std::is_same<S, std::string>::value || std::is_same<S, std::string_view>::value, "1st arg only support string or string_view");
        auto str = Trim(_str);
        if constexpr(std::is_same<S, std::string>::value) {
            if(!IsNum(str.data()))
                return false;
            to = std::stoi(str.data());
        }
        else {
            std::string _str(str);
            if(!IsNum(_str.data()))
                return false;
            to = std::stoi(_str.data());
        }
        return true;
    }
    template<typename S>
    inline bool FromString(const S& _str, float& to) {
        static_assert (std::is_same<S, std::string>::value || std::is_same<S, std::string_view>::value, "1st arg only support string or string_view");
        auto str = Trim(_str);
        if constexpr(std::is_same<S, std::string>::value) {
            if(!IsNum(str.data()))
                return false;
            to = std::stof(str.data());
        }
        else {
            std::string _str(str);
            if(!IsNum(_str.data()))
                return false;
            to = std::stof(_str.data());
        }
        return true;
    }
    template<typename S>
    inline bool FromString(const S& _str, double& to) {
        static_assert (std::is_same<S, std::string>::value || std::is_same<S, std::string_view>::value, "1st arg only support string or string_view");
        auto str = Trim(_str);
        if constexpr(std::is_same<S, std::string>::value) {
            if(!IsNum(str.data()))
                return false;
            to = std::stod(str.data());
        }
        else {
            std::string _str(str);
            if(!IsNum(_str.data()))
                return false;
            to = std::stod(_str.data());
        }
        return true;
    }
#endif

    template<typename S>
    bool FromString(const S& str, std::string& to) {
        static_assert (std::is_same<S, std::string>::value || std::is_same<S, std::string_view>::value, "1st arg only support string or string_view");
        to = std::string(str.data(), str.data()+str.size());
        return true;
    }

    template<typename S, typename _Tp>
    bool FromString(const S& str, std::vector<_Tp>& to, const std::initializer_list<char>& delim = {',', '\t', ' ', '\n'}) {
        static_assert (std::is_same<S, std::string>::value || std::is_same<S, std::string_view>::value, "1st arg only support string or string_view");
        auto split = Split(str, delim, true);
        std::vector<_Tp> tmpVec;
        tmpVec.reserve(split.size());
        for(const auto& one : split) {
            _Tp tmp;
            if(!FromString(one, tmp))
                return false;
            tmpVec.push_back(std::move(tmp));
        }
        to = std::move(tmpVec);
        return true;
    }

    //! @brief to数组中的初始值设为默认值。初始值为NAN的表示没有默认值，必须有输入，无则返回false
    template<typename S, std::size_t _Size>
    bool FromStringWithDefVal(const S& str, std::array<double, _Size>& to, const std::initializer_list<char>& delim = {',', '\t', '\n'}) {
        static_assert (std::is_same<S, std::string>::value || std::is_same<S, std::string_view>::value, "1st arg only support string or string_view");
        auto split = Split(str, delim, false);
        for(size_t i = split.size(); i < _Size; i++)
            if(std::isnan(to[i]))
                return false;
        auto minSize = std::min(split.size(), _Size);
        for(size_t i = 0; i < minSize; i++)
            if(!FromString(split[i], to[i]) && std::isnan(to[i]))
                return false;
        return true;
    }

    template<typename S, typename It>
    bool FromStringWithDefVal(const S& str, It begin, It end, const std::initializer_list<char>& delim = {',', '\t', '\n'}) {
        static_assert (std::is_same<S, std::string>::value || std::is_same<S, std::string_view>::value, "1st arg only support string or string_view");
        static_assert(std::is_floating_point<typename std::iterator_traits<It>::value_type>::value,
                      "Iterator value_type must be a floating point type (float or double).");
        auto split = Split(str, delim, false);
        auto dist = std::distance(begin, end);
        if(dist < 0)
            return false;
        auto _Size = (std::size_t)dist;
        auto itr = begin;
        for(size_t i = split.size(); i < _Size; ++i, ++itr)
            if(std::isnan(*itr)) {
                return false;
            }
        auto minSize = std::min(split.size(), (std::size_t)_Size);
        itr = begin;
        for(size_t i = 0; i < minSize; ++i, ++itr) {
            if(!FromString(split[i], *itr) && std::isnan(*itr))
                return false;
        }
        return true;
    }

    template <typename S, typename _Tp, std::size_t _Size>
    bool FromString(const S& str, std::array<_Tp, _Size>& to, const std::initializer_list<char>& delim = {',', '\t', ' ', '\n'}) {
        static_assert (std::is_same<S, std::string>::value || std::is_same<S, std::string_view>::value, "1st arg only support string or string_view");
        auto split = Split(str, delim, true);
        std::array<_Tp, _Size> tmpArr;
        auto n = split.size();
        if(n != _Size)
            return false;
        for(std::size_t i = 0; i < n; i++) {
            _Tp tmp;
            if(!FromString(split[i], tmp))
                return false;
            tmpArr[i] = tmp;
        }
        to = std::move(tmpArr);
        return true;
    }

    template <typename S>
    bool FromString(const S& str, bool& to) {
        static_assert (std::is_same<S, std::string>::value || std::is_same<S, std::string_view>::value, "1st arg only support string or string_view");
        if((ToUpper(str) == "TRUE") || (str == "1"))
            to = true;
        else if(ToUpper(str) == "FALSE" || (str == "0"))
            to = false;
        else
            return false;
        return true;
    }

    std::string ToString(long long v);
    inline std::string ToString(const std::string& str) { return str; }

    //! @brief 区别于ToString(value,precision,scientific), 以GFE_StrFormat.h中的格式输出
    std::string ToString(double v);

    //! @param precision 小数点后位数
    //! @param scientific 科学计数法表示
    std::string ToString(double v, int precision, bool scientific);

    template<class _Tp>
    typename std::enable_if_t<std::is_arithmetic_v<typename std::remove_cv_t<_Tp>>, std::string>
    ToString(_Tp v)
    {
        if constexpr (std::is_integral_v<std::remove_cv_t<_Tp>>)
            return ToString((long long)v);
        else if constexpr (std::is_floating_point_v<std::remove_cv_t<_Tp>>)
            return ToString((double)v);
    }

    template <typename _Tp>
    typename std::enable_if<std::is_arithmetic<typename std::remove_cv<_Tp>::type>::value, std::string>::type
    ToString(const std::vector<_Tp>& data, const std::string& seperator)
    {
        std::string ret;
        for(int i = 0; i < data.size()-1; i++)
            ret.append(ToString(data[i])+seperator);
        if(!data.empty())
            ret.append(ToString(data.back()));
        return ret;
    }

    template <typename _Tp, std::size_t _Size>
    typename std::enable_if<std::is_arithmetic<typename std::remove_cv<_Tp>::type>::value, std::string>::type
    ToString(const std::array<_Tp, _Size>& data, const std::string& seperator)
    {
        std::string ret;
        for(int i = 0; i < _Size-1; i++)
            ret.append(ToString(data[i])+seperator);
        if(_Size != 0)
            ret.append(ToString(data.back()));
        return ret;
    }

    template<typename _Tp>
    typename std::enable_if<std::is_pointer<_Tp>::value, std::string>::type
    toString(const _Tp data, std::size_t pos = -1){
        if(pos == -1)
            return std::string(data);
        else
            return std::string(data, data+pos);
    }

    template <typename _Clock, typename... _Args>
    std::string toString(const std::chrono::time_point<_Clock, _Args...>& tm){
        using namespace std::chrono;
        auto sctp = time_point_cast<system_clock::duration>(tm - _Clock::now()
              + system_clock::now());
        auto tt = system_clock::to_time_t(sctp);
        return std::asctime(std::localtime(&tt));
    }

    template <typename _Tp>
    std::string toString(const std::complex<_Tp>& number){
        if(number.real() == 0){
            if(number.imag() == 0)
                return 0;
            else
                return ToString(number.imag()) + "i";
        }
        else {
            if(number.imag() == 0)
                return ToString(number.real());
            else if(number.imag() > 0)
                return ToString(number.real()) + "+" + std::to_string(number.imag()) + "i";
            else
                return ToString(number.real()) + std::to_string(number.imag()) + "i";
        }
    }
}

#if _WIN32
inline std::wstring operator ""_cp (const char* str, std::size_t) {
    return GFE::ToWString(str);
}
#else
inline std::string operator ""_cp (const char* str, std::size_t) {
    return std::string(str);
}
#endif
