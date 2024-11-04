#ifndef GFE_STRING_H
#define GFE_STRING_H

#include "GFE_StrUtil.h"

namespace GFE::String {

struct PLACE_HOLDER {};

namespace {
template <typename T>
struct has_operator_star {
    template <typename C> static char test(decltype (&C::operator*));
    template <typename C> static int test(...);
    static constexpr auto value = sizeof (test<T>(nullptr)) == sizeof(char);
};

template <typename S, typename _Tp>
static bool str_to_data(S&& s, _Tp& value) {
    GFE::FromString(GFE::Trim(std::forward<S>(s)), value);
    return true;
}
template <typename S>
static bool str_to_data(S&&, PLACE_HOLDER&) { return true; }
}

// 按序将[begin, end)内字符串转换为相应数据
// 超出字符串个数的数据将不会转换
template <typename It, typename... Args>
void Todata(It begin, It end, Args&& ...values) {
    ((begin != end && str_to_data(*begin++, values))&&...);
}
template <typename InputIt, typename OutputIt>
std::enable_if_t<has_operator_star<OutputIt>::value || std::is_pointer_v<OutputIt>, void>
Todata(InputIt ibegin, InputIt iend, OutputIt obegin, OutputIt oend) {
    while (ibegin != iend && obegin != oend)
        GFE::FromString(GFE::Trim(*ibegin++), *obegin++);
}
// wrapper
template <typename _Tp, typename... Args>
void Todata(const _Tp& fields, Args&& ...values) {
    Todata(fields.cbegin(), fields.cend(), values...);
}
template <typename _Tp, typename It>
std::enable_if_t<has_operator_star<It>::value, void>
Todata(const _Tp& fields, It begin, It end) {
    Todata(fields.cbegin(), fields.cend(), begin, end);
}

}

#endif	// GFE_STRING_H
