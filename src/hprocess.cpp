
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>

#include "hstr.h"

#include "hprocess.h"
#include "hstr.h"



namespace HUICPP {

bool HProcess::ProcessExists (const pid_type& pid) {

	HSTR str = HStr::Format("/proc/%d", pid);

	struct stat st = {0};
	return stat(str.c_str(), &st) == 0;

}


HRET HProcess::KillProcess (const pid_type& pid) {

#ifdef HWINDOWS
	HCStr str = HCStr::Format("kill -15 %d", (int)pid);
	system(str.c_str());
	//(void)execl("/usr/bin/kill", "kill", "15", HCStr::itos(pid).c_str(), nullptr);
	//(void)execl("/bin/kill", "kill", "15", HCStr::itos(pid).c_str(), nullptr);
#else
	auto cb = kill(pid, SIGTERM);
	HASSERT_RETURN(cb == 0, SYS_FAILED);
#endif 
	
	HRETURN_OK;

}


HRET HProcess::GetProcName(HSTRR strName) {

    HCSZ csz = "/proc/self/exe";

	HNAME_BUF buf = {0};

	HASSERT_RETURN(readlink(csz, buf, HNAME_LEN) >= 0, SYS_FAILED);

	strName.assign(buf);

    HRETURN_OK;

}


}










