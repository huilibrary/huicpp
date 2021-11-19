
#include "hthreadpool.h"

namespace HUICPP {

HThreadPool::HThreadPool() noexcept
    : base(), m_pinfos(nullptr), m_mutex() {  }



HThreadPool::HThreadPool(HUN uChildCount) noexcept 
    : base(uChildCount), m_pinfos(nullptr), m_mutex() {
    
    create_son_infos(0, GetChildCount());

}


HThreadPool::~ HThreadPool () {

    free_sons(GetChildCount());

}



void HThreadPool::SetChildCount(HUN nChildCount) {

    create_son_infos(GetChildCount(), nChildCount);
    base::SetChildCount(nChildCount);

}



HRET HThreadPool::Init () {

    thread_arg * parr = static_cast<thread_arg*>(malloc(
        sizeof(thread_arg) * GetChildCount()));
    ZERO_MEM(parr, sizeof(thread_arg) * GetChildCount());

    HThread thread;
    for (HUN i = 0; i < GetChildCount(); ++i) {

        parr[i].m_index = i;
        parr[i].m_pool = this;

        HNOTOK_THROW(thread.Create(thread_init_run, &(parr[i])));

        s_son_lock.WaitSonRun(s_uChildCreated);

        m_pinfos[i].m_thread = std::move(thread);
        m_pinfos[i].m_cs = ChildStatus::CS_IDLE;

    }

    free(parr);
    HRETURN_OK;

}



void HThreadPool::free_sons (HUN uChildCount) {

    // free threads first.
    for (HUN i = 0; i < uChildCount; ++i) {

        if (m_pinfos[i].m_cs == ChildStatus::CS_UNUSED) {
            continue;
        }

        HIGNORE_RETURN(m_pinfos[i].m_thread.Cancel());

        HIGNORE_RETURN(m_pinfos[i].m_thread.Join(nullptr));

    }

    HDEL_ARR(m_pinfos);

}


void HThreadPool::create_son_infos(HUN uOldChildCount, HUN uNewChildCount) {

    // if nullptr, we do nothing.
    if (m_pinfos == nullptr) {
        return ;
    }


    // free threads first.
    free_sons(uOldChildCount);

    m_pinfos = new child_info[uNewChildCount];
    CHECK_NEWPOINT(m_pinfos);

    for (HUN i = 0; i < uNewChildCount; ++i) {

        m_pinfos[i].m_cs = ChildStatus::CS_UNUSED;

    }

}


void* HThreadPool::thread_init_run(void* argv) {

    thread_arg* arg = static_cast<thread_arg*>(argv);
    HThreadPool* pool = arg->m_pool;
    HUN index = arg->m_index;

    HASSERT_THROW(pool != nullptr, INVL_PARA);

    s_son_lock.SonRun(s_uChildCreated);

    pool->childThreadRun(index);

    return nullptr;

}


HWaitSonLock HThreadPool::s_son_lock;
HThreadKey HThreadPool::s_thread_key;
HThreadOnce HThreadPool::s_thread_once;
HN HThreadPool::s_uChildCreated = 0;


void HThreadPool::ThreadSpecifiOnce () {

    s_thread_key.Create(ThreadSpecifiDestructor);

}


void HThreadPool::ThreadSpecifiDestructor(void* ptr) {

    HSZ pt = static_cast<HSZ>(ptr);
    HDEL_ARR(pt);

}


HSZ HThreadPool::CreateThreadSpecific(HUN len) {

    HSZ res = nullptr;
    s_thread_once.Once(ThreadSpecifiOnce);

    if ((res = static_cast<HSZ>(s_thread_key.Get())) == nullptr) {

        res = new HCH[len];
        HNOTOK_THROW(s_thread_key.Set(res));

        memset(res, 0, len);

    }

    return res;

}


}