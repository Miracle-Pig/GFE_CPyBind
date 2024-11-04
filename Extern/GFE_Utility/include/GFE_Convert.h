#ifndef GFE_CONVERT_H
#define GFE_CONVERT_H

namespace GFE {

    // wrapper

    template <typename _Tp1, typename _Tp2>
    typename std::enable_if<std::is_convertible<_Tp1, _Tp2>::value, _Tp1>::type
    ConvertTo(const _Tp2& data){
        return _Tp1(data);
    }

    template <typename _Tp1, typename _Tp2>
    typename std::enable_if<std::is_convertible<typename _Tp1::value_type, typename _Tp2::value_type>::value, _Tp1>::type
    ConvertTo(const _Tp2& data){
        return _Tp1(data.cbegin(), data.cend());
    }
    template <typename _Tp1, typename _Tp2>
    typename std::enable_if<std::is_convertible<typename _Tp1::value_type, typename _Tp2::value_type>::value, _Tp1>::type
    ConvertTo(_Tp2&& data){
        return _Tp1(std::make_move_iterator(data.begin()), std::make_move_iterator(data.end()));
    }

}


#endif // GFE_CONVERT_H
