

#include "hmessagequeue.h"


namespace HUICPP {

HMessageQueue::HMessageQueue() noexcept
	: base() {
}


HMessageQueue::HMessageQueue(key_type key, flag_type flag) noexcept
	: base (key, flag) {
}


HMessageQueue::~HMessageQueue() {

}


HRET HMessageQueue::Remove () {

	return control(IPC_RMID);

}


HRET HMessageQueue::Send (HCPTR ptr, size_type size, flag_type flags) {

	HASSERT_RETURN(msgsnd(GetId(), ptr,  size, flags) == 0, SYS_FAILED);
    
	HRETURN_OK;

}


HRET HMessageQueue::Recv (HPTR buf, HSIZER size, HLN lType, flag_type flags) {

    auto cb = msgrcv(GetId(), buf, size, lType, flags);
	HASSERT_RETURN(cb >= 0, SYS_FAILED);

	size = cb;

	HRETURN_OK;

}


HRET HMessageQueue::create () {

	if (HasInited()) {
        HRETURN_OK;
    }

	SYS_RET_T msgid = msgget(GetKey(), GetFlag() | IPC_CREAT | IPC_EXCL | 600);
    
    HASSERT_RETURN(msgid > 0 or errno != EEXIST, SRC_ERR);
    HASSERT_RETURN(msgid > 0, SYS_FAILED);

    SetId(msgid);
    
    HRETURN_OK;

}


HRET HMessageQueue::open () {

	if(HasInited()) {
        HRETURN_OK;
    }

    SYS_RET_T msgid = msgget(GetKey(), GetFlag());
    HASSERT_RETURN(msgid > 0, SYS_FAILED);

    SetId(msgid);

    HRETURN_OK;

}


HRET HMessageQueue::control (cmd_type cmd, struct msqid_ds* mds) {

	HASSERT_RETURN(HasInited(), SRC_ERR);

    HASSERT_RETURN(msgctl(GetId(), cmd, mds) >= 0, SYS_FAILED);

    HRETURN_OK;

}


}



