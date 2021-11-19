/*
* Created: Joseph Hui Wed 29 Sep 2021
* Description:
    Wrapper for shared memory.
*/


#ifndef __H_HUICPP_SHARE_MEMORY_H__
#define __H_HUICPP_SHARE_MEMORY_H__

#include "huicpp.h"
#include "internal/ipcbase_in.h"

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

namespace HUICPP {

using share_memory_base = hc_internal::ipc_basic<SYS_T>;

class HShareMemory : public share_memory_base {
public:
    using base = share_memory_base;

public:
    HShareMemory() noexcept;

    HShareMemory(key_type key, flag_type flag, size_type shmsize) noexcept;
	
    ~ HShareMemory();

public:
    size_type GetShmSize() const noexcept { return m_size; }

    HSZ GetShmPtr () noexcept { return m_shm_ptr; }

    HCSZ GetShmPtr () const noexcept { return m_shm_ptr; }

public:
    virtual HRET Remove() override;

protected:
    // not thread-safe.
    virtual HRET create() override;

    virtual HRET open() override;

    HRET attach(HCPTR virtual_addr = nullptr, flag_type flag = 0);

    HRET dettach();

private:
    HRET control(cmd_type cmd, struct shmid_ds * ds = nullptr);


private:
    size_type m_size;

    HSZ m_shm_ptr;

};

}

#endif // __H_HUICPP_SHARE_MEMORY_H__

