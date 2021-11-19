

#include "hlog.h"
#include "htime.h"
#include "hstr.h"
#include "utils/base_utils.h"

#include <cstdarg>
#include <cstdlib>
#include <iostream>

#include <fcntl.h>
#include <unistd.h>


namespace HUICPP {

HLog::HLog(HN settings, HSIZE nShiftSize) :
	m_strFileLogName(), 
	m_settings(settings),
	m_len(0), m_nDayIndex(0), 
	m_cnShiftSize(nShiftSize), m_fd(INVALID_FD),
	m_logTime(time(nullptr)), m_mutex(nullptr) {

	initlog();

}

HLog::HLog(HCSTRR strFileName, HN settings, HSIZE nShiftSize)
	: m_strFileLogName(strFileName), 
	m_settings(settings),
	m_len(0), m_nDayIndex(0), 
	m_cnShiftSize(nShiftSize), m_fd(INVALID_FD), 
	m_logTime(time(nullptr)), m_mutex(nullptr)  {

	initlog();

}

HLog::~HLog() {

	uninitlog();

}


// for windows vscode.
// if you work in difference ENV, you can comment follows.[_VA_LIST_DEFINED]
#ifndef _VA_LIST_DEFINED
typedef __gnuc_va_list va_list;
#define va_start(v,l)	__builtin_va_start(v,l)
#define va_end(v)	__builtin_va_end(v)
#define va_arg(v,l)	__builtin_va_arg(v,l)
#define _VA_LIST_DEFINED
#endif


HRET HLog::WriteLog(HCSTRR log) {

	log_locker_holder holder(m_mutex);

	if (utils::has_bit(m_settings, LOG_SHIFTLOG)) {

		shiftLog();

		if (not IS_VALID_FD(m_fd)) {

			init();

		}

		m_len += log.length();

	}

	ssize_t wcb = write(m_fd, log.c_str(), log.length());
	if (wcb < 0) {

		printf("write failed");

	}

	HRETURN_OK;
}


HSTR HLog::newLogName() {

	return m_strFileLogName + "_" + HTime::GetCurrentTime().ToString("%Y%m%d") + "_" +
		  HStr::FromN(m_nDayIndex) + ".log";

}


void HLog::shiftLog() {

	if (more_day()) {

		m_nDayIndex = 0;

		m_len = 0;

		init();

		return;

	}

	if (m_len > m_cnShiftSize) {

		++m_nDayIndex;

		init();

		return;

	}

}


void HLog::initlog() {

	if (utils::has_bit(m_settings, LOG_LOCKWRITE)) {
		m_mutex = new pthread_mutex_t;
		CHECK_NEWPOINT(m_mutex);
		HIGNORE_RETURN(pthread_mutex_init(m_mutex, nullptr));
	}

	init();

}



void HLog::uninitlog() {

	if (utils::has_bit(m_settings, LOG_LOCKWRITE)) {
		if (m_mutex != nullptr) {
			HIGNORE_RETURN(pthread_mutex_destroy(m_mutex));
		}
	}

	uninit();

}


void HLog::init() {

	if (m_strFileLogName.empty()) {
		return ;
	}

	// before initalize the log file descriptor,
	// We need to make sure it has been uninitialized.
	if (IS_VALID_FD(m_fd)) {

		uninit();

	}

	m_fd = ::open(newLogName().c_str(), O_CREAT | O_WRONLY | O_APPEND, 0664);
	if (not IS_VALID_FD(m_fd)) {

		std::cout << "fopen failed" << std::endl;
		exit(-1);

	}

	m_len = 0;

}

void HLog::uninit() {

	m_len = 0;
	m_nDayIndex = 0;
	CLOSE_FD(m_fd);

}



HRET HLog::write_log(HCSZ ff, HN ll, HCSZ slog, HCSZ stype) {

	log_buf_t buf = {0};
	ZERO_MEM(buf, LOG_LEN);	

	if (stype != nullptr) {

#ifdef HWIN32
	snprintf(buf, LOG_LEN, "[%s][%d][%s]: %s.[%s:%d]\n", 
		HTime::GetNow().c_str(), (int)getpid(), stype, 
		slog, ff, ll);
#else
	HIGNORE_RETURN(snprintf(buf, LOG_LEN, "[%s][%d][%lu][%s]:%s[%s:%d]\n", 
		HTime::GetNow().c_str(), (int)getpid(), (unsigned long)pthread_self(), stype, 
		slog, ff, ll));
#endif

	} else {

#ifdef HWIN32
	snprintf(buf, LOG_LEN, "[%s][%d]: %s.[%s:%d]\n", 
		HTime::GetNow().c_str(), (int)getpid(), slog, ff, ll);
#else
	HIGNORE_RETURN(snprintf(buf, LOG_LEN, "[%s][%d][%lu]:%s[%s:%d]\n", 
		HTime::GetNow().c_str(), (int)getpid(), (unsigned long)pthread_self(), 
		slog, ff, ll));
#endif

	}

	log_locker_holder holder(m_mutex);

	if (utils::has_bit(m_settings, LOG_SHIFTLOG)) {

		shiftLog();

		if (not IS_VALID_FD(m_fd)) {

			init();

		}

		m_len += strlen(buf);

	}

	ssize_t wcb = write(m_fd, buf, strlen(buf));
	if (wcb < 0) {

		printf("write failed");

	}

	HRETURN_OK;
}


bool HLog::more_day() {

	time_t _tt = time(nullptr);
	struct tm *p = localtime(&_tt);
	int ny = p->tm_year, nm = p->tm_mon, nd = p->tm_mday;

	p = localtime(&m_logTime);
	int ly = p->tm_year, lm = p->tm_mon, ld = p->tm_mday;

	if (ny == ly && nm == lm && nd == ld) {
		return false;
	}

	m_logTime = _tt;

	return true;
}


HLog::log_locker_holder::log_locker_holder(pthread_mutex_t* p) 
	: pmutex(p) {
		
	if (pmutex != nullptr) {
		pthread_mutex_lock(pmutex);
	}

}


HLog::log_locker_holder::~log_locker_holder() {

	if (pmutex != nullptr) {
		pthread_mutex_unlock(pmutex);
	}

}


HLogManager::HLogManager () noexcept
	: m_settings(LOG_ONLY_ONE_DEFAULT), m_nShiftSize(HLog::DEFAULT_LOG_LENGTH),
		m_strName(), m_logs{0} {

}


HLogManager::~HLogManager () {

	uninit();

}


void HLogManager::Setup(HCSTRR strName, HN settings, HSIZE nShiftSize) {

	m_nShiftSize = nShiftSize;

	m_settings = settings;	

	m_strName = strName;

	init();

}


HLog* HLogManager::GetLog (lm_index index) {

	HASSERT_THROW(index <= lm_end, OVER_FLOW);

	HN ind = index;

	HLog* res = m_logs[ind];
	if (res == nullptr) {
		if (index < lmn_normal) {
			// system
			ind += 3;
			res = m_logs[ind];		

		}
	}

	if (res == nullptr) {
		res = m_logs[lmn_normal];
	}

	return res;

}


void HLogManager::WriteLog(lm_index index, HCSTRR log) {

	HASSERT_THROW(index <= lm_end, OVER_FLOW);

	HLog* plog = GetLog(index);
	CHECK_NEWPOINT(plog);

	HIGNORE_RETURN(plog->WriteLog(log));

}


HSTR HLogManager::GetFlagsStr (lm_index index) {

	HASSERT_THROW(index <= lm_end, OVER_FLOW);

	switch (index) {
		case lms_normal:
			return "SNORMAL";	
			break;

		case lms_warn:
			return "SWARNING";		
			break;

		case lms_error:
			return "SERROR";	
			break;
		
		case lmn_normal:
			return "NNORMAL";
			break;

		case lmn_warn:
			return "NWARNING";
			break;

		case lmn_error:
			return "NERROR";
			break;
	
		default:
			return "SYSTEM_ERROR";
			break;
	}

	return "SYSTEM_ERROR";
}


#define INIT_LOG_MS(fmt,index) do { \
	strName = HStr::Format(fmt, m_strName.c_str()); \
	HN log_settings = HLog::LOG_NORMAL_SETTINGS; \
	if (index < lmn_normal) {	\
		if (utils::has_bit(m_settings, LOG_SYSTEM_WITHLOCK)) {	\
			log_settings = log_settings | HLog::LOG_LOCKWRITE;	\
		} 	\
		if (utils::has_bit(m_settings, LOG_SYSTEM_SHIFT)) {	\
			log_settings = log_settings | HLog::LOG_SHIFTLOG;	\
		} 	\
	} else {	\
		if (utils::has_bit(m_settings, LOG_NORMAL_WITHLOCK)) {	\
			log_settings = log_settings | HLog::LOG_LOCKWRITE;	\
		} 	\
		if (utils::has_bit(m_settings, LOG_NORMAL_SHIFT)) {	\
			log_settings = log_settings | HLog::LOG_SHIFTLOG;	\
		} 	\
	}	\
	m_logs[index] = new HLog(strName, log_settings, m_nShiftSize); \
	CHECK_NEWPOINT(m_logs[index]); } while(0)

void HLogManager::init () {

	for (HN i = 0; i < lm_end; ++i) {
		m_logs[i] = nullptr;
	}
	
	HSTR strName;
	// if LOG_ONLY_ONE_DEFAULT has set, we only new a normal-normal log.
	if(m_settings == LOG_ONLY_ONE_DEFAULT) {
		INIT_LOG_MS("%s", lmn_normal);
		return;
	}

	if (utils::has_bit(m_settings, LOG_WITH_SYSTEM)) {

		if(utils::has_bit(m_settings, LOG_SYSTEM_MULTIPLE)) {

			INIT_LOG_MS("%s_system_normal",lms_normal);

			INIT_LOG_MS("%s_system_warn",lms_warn);

			INIT_LOG_MS("%s_system_error",lms_error);

		} else {

			INIT_LOG_MS("%s_system",lms_normal);

		}
	}

	if (utils::has_bit(m_settings, LOG_NORMAL_MULTIPLE)) {

		INIT_LOG_MS("%s_normal",lmn_normal);

		INIT_LOG_MS("%s_warn",lmn_warn);

		INIT_LOG_MS("%s_error",lmn_error);

	}

	if(m_logs[lmn_normal] == nullptr) {
		INIT_LOG_MS("%s",lmn_normal);
	}	

}

#undef INIT_LOG_MS


void HLogManager::uninit() {

	for (HN i = 0; i < lm_end; ++i) {

		HLog* p = m_logs[i];
		HDELP(p);

	}

	ZERO_MEM(m_logs, 0);

}


HLogManager g_log_manger;
HLogManager* gp_manager = nullptr;

HRET InitLog(HCSTRR strFilename, HN settings, HSIZE nShiftSize) {

	g_log_manger.Setup(strFilename, settings, nShiftSize);

	gp_manager = &g_log_manger;

	HRETURN_OK;
}


void WriteLog(HLogManager::lm_index index, HCSZ ff, HN ll, HCSZ fmt, ...) {
	
	HASSERT_THROW(index <= HLogManager::lm_end, OVER_FLOW);

	HLog::log_buf_t log_buf = {0};
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(log_buf, HLog::LOG_LEN, fmt, ap);
	va_end(ap);

#ifdef HWIN32
	HLog::log_buf_t buf = {0};
	HUN line_len =  LOG_LEN;
#else 
	static constexpr const HN buf_line_buf_len = HSTRING_LEN + 128;
	typedef HCH buf_line_buf_t[buf_line_buf_len];
	buf_line_buf_t buf = {0};
	HUN line_len =  buf_line_buf_len;
#endif 

#ifdef HWIN32
	snprintf(buf, HLog::LOG_LEN, "[%s][%d][%s]: %s.[%s:%d]\n", 
		HTime::GetNow().c_str(), (int)getpid(), GetFlagsStr(index).c_str(), 
		log_buf, ff, ll);
#else
	HIGNORE_RETURN(snprintf(buf, line_len, 
		"[%s][%d][%lu][%s]:%s[%s:%d]\n", 
		HTime::GetNow().c_str(), 
		(int)getpid(), (unsigned long)pthread_self(), 
		HLogManager::GetFlagsStr(index).c_str(), 
		log_buf, ff, ll));
#endif

	g_log_manger.WriteLog(index, HSTR(buf));

}


} // namespace HUIBASE
