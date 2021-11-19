/**********************************************************
 * Created: Joseph Sep-16-2021
 * Description: 
 * 		Wrapper for huicpp todo holder.
 **********************************************************/


#ifndef __H_HUICPP_TODOHOLDER_H__
#define __H_HUICPP_TODOHOLDER_H__

#include "huicpp.h"


template<class df, class udf>
struct TodoHolder {
    TodoHolder(df df_ptr, udf udf_ptr) 
        : m_udf(udf_ptr) {
        df_ptr();   
    }

    ~ TodoHolder() {
        m_udf();
    }
    udf m_udf;
};


template<class doitfun>
struct DoitHolder {
    DoitHolder(doitfun ff) : m_fun(ff) {  }

    ~ DoitHolder() {
        m_fun();
    }

    doitfun m_fun;
};
#define DOIT_FUN(fun) DoitHolder<decltype(fun)> doit_ ## fun ## _holder(fun)


#endif // __H_HUICPP_TODOHOLDER_H__

