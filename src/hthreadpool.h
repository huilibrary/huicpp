/*
 * Created: Joseph Hui Thu Sep 21 2017
 * Description: 
  		Wrapper for huicpp thread pool.
*/


#ifndef __H_HUICPP_THREADPOOL_H__
#define __H_HUICPP_THREADPOOL_H__

#include "internal/pool.h"
#include "hthread.h"
#include "hmutex.h"
#include "hthreadkey.h"
#include "hthreadonce.h"

namespace HUICPP {

class HThreadPool  : public hc_internal::Pool {
public:
   using base = hc_internal::Pool;

   typedef struct _child_info {
      ChildStatus m_cs;
      HThread m_thread;

      _child_info () : m_cs(ChildStatus::CS_UNUSED), m_thread() { }
   } child_info;

   typedef struct _thread_arg {
      HThreadPool* m_pool;
      HUN m_index;
   } thread_arg;


public:
   HThreadPool() noexcept;

   explicit HThreadPool(HUN uChildCount) noexcept;
   
    virtual ~ HThreadPool ();

public:
   virtual void SetChildCount(HUN nChildCount) override;

    virtual HRET Init () override;

    virtual HRET ManageChild ();

    virtual HRET SetChildStatus(HUN chindIndex, ChildStatus status) = 0;

protected:
   virtual void childThreadRun (HUN uChildIndex) = 0;

private:
   void free_sons (HUN uChildCount);

   void create_son_infos(HUN uOldChildCount, HUN uNewChildCount);

private:
   static void* thread_init_run(void* argv);   

public:
   static HWaitSonLock s_son_lock;
   static HThreadKey s_thread_key;
   static HThreadOnce s_thread_once;
   static HN s_uChildCreated;

   static void ThreadSpecifiOnce ();
   static void ThreadSpecifiDestructor(void* ptr);
   static HSZ CreateThreadSpecific(HUN len);


protected:
   child_info* m_pinfos;
   HMutex m_mutex;
};


}
#endif //__H_HUICPP_THREADPOOL_H__
