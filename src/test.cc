
#include "huicpp.h"
#include <iostream>

#include "htime.h"
#include "hlog.h"
#include "hmutex.h"
#include "haddr.h"
#include "hsocket.h"
#include "hsingleton.hpp"
#include "hmstr.h"
#include "hapifactory.h"
#include "hfilestr.h"
#include "hnormalconf.h"
#include "hjsonconf.h"
#include "hconf.h"
#include "hfilelock.h"
#include "happlock.h"
#include "hfilename.h"
#include "hsharememory.h"
#include "hmessagequeue.h"
#include "hsemaphore.h"

using namespace HUICPP;

using sock_ins = HSingleton<HSocket>;

using int_api_factory = HApiFactory<int>;

int main() {

	HIGNORE_RETURN(InitLog("testapp", HLogManager::LOG_NORMAL_MULTIPLE));
	HMutex mutex;

	HFileStr fs;

	HNormalConfig normal_conf;
	HJsonConfig json_conf;

	HVSTR strs;
	HIGNORE_RETURN(HIp4Addr::GetLocalIps(strs));
	for (HCSTRR s : strs) {
		std::cout << s << std::endl;
	}

	HTcpSocket tcp;

	HMstr mstr;

	HConfig h_config;
	HCond cond;

	HWaitSonLock wslock;

	HFileName _fn;
	_fn.Exists();

	LOG_NS("hello tom");

	const char* csz = "aaa";
	LOG_ERROR("csz:%s", csz);
	SLOG_ERROR("csz:%s", csz);

	std::cout << hlast_err() << std::endl;
	HASSERT_THROW(1==1, INVL_PARA);

	HTimeSpan ts;
	std::cout << ts.GetTimeSpan() << std::endl;

	HShareMemory shmemory;
	HMessageQueue hmqueue;
	HSemaphore HSEM_ipcs;
	
	HTime tt;
	tt.SetAsNow();
	std::cout << tt.GetDay() << std::endl;
	std::cout << HTime::GetNow() << std::endl;
	std::cout << HTime::GetDate() << std::endl;
	std::cout << HTime::GetPrevTimeGroup() << std::endl;
	std::cout << HTime::GetCurrentTime().ToString("%Y%m%d") << std::endl;

	return 0;
}
