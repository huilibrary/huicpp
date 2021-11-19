/**********************************************************
 * Created: Joseph Sep-06-2021
 * Description: 
 *      Define some basic utils.
 **********************************************************/


#ifndef __H_BASE_UTILS_H__
#define __H_BASE_UTILS_H__

#include "../huicpp.h"

namespace HUICPP {

namespace utils {

inline HN uper_divied(HN n, HN b) {

    return n / b + n % b > 0 ? 1 : 0;

}


template<class _Ty>
inline auto remove_bit(_Ty& s, _Ty v) -> decltype(s) {
    return s & ~v;
}


template<class _Ty>
inline auto set_bit(_Ty& s, _Ty v) -> decltype(s) {
    return s & v;
}


template<class _Ty>
inline bool has_bit(_Ty s, _Ty v) {
    return s & v;
}


inline bool has_bit(HN s, HN v) {
    return s & v;
}


template<class _Ty>
inline auto get_bit(_Ty s, _Ty v) -> decltype(s) {
    return s & v;
}




}

}

#endif //__H_BASE_UTILS_H__

