



#include "happlock.h"
#include <unistd.h>
#include <sys/file.h>


namespace HUICPP {

HAppLock::HAppLock(HCSTRR strFileName) 
    : HFileHandle(strFileName, O_CREAT | O_RDWR, 0644) {

}


HAppLock::~ HAppLock() {

    HIGNORE_RETURN(UnlockApp());

}



HRET HAppLock::LockApp () const {

    HASSERT_RETURN(lockf(GetHandle(), F_TEST, 0) == 0, INVL_PARA);

    HASSERT_RETURN(lockf(GetHandle(), F_LOCK, 0) == 0, SYS_FAILED);

    HRETURN_OK;

}


HRET HAppLock::UnlockApp () const {

    HASSERT_RETURN(lockf(GetHandle(), F_UNLCK, 0) == 0, SYS_FAILED);

    HRETURN_OK;

}



HProcessLock::HProcessLock(HCSTRR strFileName) 
    : HFileHandle(strFileName, O_CREAT | O_RDWR, 0644) {

    HIGNORE_RETURN(::flock(GetHandle(), LOCK_EX));

}

HProcessLock::~HProcessLock() {

    ::flock(GetHandle(), LOCK_UN);

}



}

