/*
 * Created: Joseph Hui Mon Nov 08 2019
 * Description: 
  		Wrapper for abstract pool.
*/


#ifndef __H_HUICPP_POOL_H__
#define __H_HUICPP_POOL_H__

#include "basictypes_in.h"
#include "commonmacro_in.h"

namespace HUICPP {

namespace hc_internal {


class Pool {
public:
    static constexpr HUN MAX_CHILD_COUNT = 1024;
    static constexpr HUN MAIN_SLEEP_TIMEVAL = 1;
    static constexpr HN MAX_QUIT_WAITOUT = 30;

    enum class ChildStatus {
        CS_UNUSED,
        CS_IDLE,
        CS_BUSY
    };

public:
    Pool () noexcept;

    explicit Pool (HUN nChildNum) noexcept;

    virtual ~ Pool ();

public:
    virtual HRET Init () = 0;

public:
    virtual void SetChildCount(HUN nChildCount) { m_nChildCount = nChildCount; }

    HUN GetChildCount () const { return m_nChildCount; }

private:
    HUN m_nChildCount;

};

}

}

#endif // __H_HUICPP_POOL_H__
