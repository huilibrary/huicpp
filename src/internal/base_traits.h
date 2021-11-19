/*
* Created: Joseph Hui Tue 24 Aug 2021
* Description:
    Sinple intergral traits declaration.
*/


#ifndef __H_HUICPP_INTERGRAL_TRAITS_IN_H__
#define __H_HUICPP_INTERGRAL_TRAITS_IN_H__

#include "basictypes_in.h"

namespace HUICPP {

namespace hc_internal{

template <class _Ty>
struct base_traits {

    using value_type = _Ty;
    using size_type = HSIZE;

    using pointer = value_type*;
    using const_pointer = const value_type*;
    using refference_type = value_type&;
    using const_refference_type = const value_type&;

    using index_type = HN;
    using itype = HN;
    using ntype = HUN;

    static constexpr value_type GetInitValue() {
        return 0;
    }

    static constexpr value_type GetDefaultValue() {
        return 0;
    }

};

}

}


#endif //__H_HUICPP_INTERGRAL_TRAITS_IN_H__
