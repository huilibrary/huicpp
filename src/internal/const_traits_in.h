/**********************************************************
 * Created: Joseph Mar-26-2021
 * Description: 
 *      Define constance traits.
 **********************************************************/


#ifndef __H_HUICPP_CONSTANCE_TRAITS_IN_H__
#define __H_HUICPP_CONSTANCE_TRAITS_IN_H__

#include "basictypes_in.h"

namespace HUICPP {

namespace hc_internal {

template<class _Ty, _Ty _Val> 
struct const_traits {
    using value_type = _Ty;
    static constexpr value_type value = _Val;

    constexpr value_type operator () () const constexpr {
        return value;
    }
};

template<bool _Val>
using bool_const_type = const_traits<bool, _Val>;

using true_const_type = bool_const_type<true>;
using false_const_type = bool_const_type<false>;

}

}

#endif // __H_HUICPP_CONSTANCE_TRAITS_H__
