/*
* Created: Joseph Hui Wed 29 Sep 2021
* Description:
    Wrapper for IPC Base.
*/


#ifndef __H_HUICPP_IPC_BASE_H__
#define __H_HUICPP_IPC_BASE_H__

#include "base_traits.h"

namespace HUICPP {

namespace hc_internal {

template<class _Ty>
struct ipc_traits : public base_traits<_Ty> {
    using base = base_traits<_Ty>;
    using value_type = typename base::value_type;

    using key_type = SYS_T;
    using flag_type = SYS_T;
    using cmd_type = HN;

    static constexpr const key_type DEF_KEY_VAL = -1;
    static constexpr const flag_type DEF_FLAG_VAL = 0;
    static constexpr const cmd_type DEF_CMD_VAL = 0;
};


template<class _Ty, class _Traits = ipc_traits<_Ty> > 
class ipc_basic {
public:
    using traits_type = _Traits;
    using value_type = typename traits_type::value_type;
    using size_type = typename traits_type::size_type;


    using key_type = typename traits_type::key_type;
    using flag_type = typename traits_type::flag_type;
    using cmd_type = typename traits_type::cmd_type;

public:
    constexpr ipc_basic() noexcept
        : m_key(traits_type::DEF_KEY_VAL), 
            m_id(traits_type::GetDefaultValue()),
            m_flag(traits_type::DEF_FLAG_VAL) {  }

    constexpr ipc_basic(key_type key, flag_type flag) noexcept
        : m_key(key), m_id(traits_type::GetDefaultValue()),
        m_flag(flag){  }

    virtual ~ ipc_basic() {  }

public:
    virtual HRET Init () {
        IF_NOTOK(create()) {
            HNOTOK_RETURN(open());
        }
        HRETURN_OK;
    }

    virtual HRET Remove() = 0;

protected:
    virtual HRET create () = 0;

    virtual HRET open () = 0;

public:
    key_type GetKey() const noexcept { return m_key; }

    value_type GetId() const noexcept { return m_id; }

    flag_type GetFlag () const noexcept { return m_flag; }

    bool HasInited() const noexcept { return m_id != traits_type::GetDefaultValue(); }

    void SetInvalid() { m_id = traits_type::GetDefaultValue(); }

    void SetKey (key_type key) { m_key = key; }

    void SetId (value_type id) { m_id = id; } 

    void SetFlag (flag_type flag) { m_flag = flag; }

protected:
    key_type m_key;
    value_type m_id;
    flag_type m_flag;
    
};


}

}


#endif // __H_HUICPP_IPC_BASE_H__

