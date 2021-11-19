/*
 * Created: Joseph Hui Tue Dec 27 2016
 * Description: 
  		Wrapper for huicpp log.
*/

#ifndef __H_HUICPP_LOG_H__
#define __H_HUICPP_LOG_H__

#include "huicpp.h"
#include <pthread.h>

namespace HUICPP {

class HLog {
public:
	enum HLOG_SETTINGS {
		LOG_NORMAL_SETTINGS = 0,
		LOG_LOCKWRITE = 1 << 0,
		LOG_SHIFTLOG = 1 << 1,
	};

public:
	using log_buf_t = HSTRING_BUF;
	static constexpr HUN LOG_LEN = HSTRING_LEN;
	static constexpr HSIZE DEFAULT_LOG_LENGTH = 1024 * 1024 * 100;		// 100M

public:
	HLog(HN settings = LOG_LOCKWRITE | LOG_SHIFTLOG, HSIZE nShiftSize = DEFAULT_LOG_LENGTH);

	HLog(HCSTRR strFileName, HN settings = LOG_LOCKWRITE | LOG_SHIFTLOG, HSIZE nShiftSize = DEFAULT_LOG_LENGTH);

	virtual ~HLog();

public:
	void SetFileLogName(HCSTRR strFileName) {
		m_strFileLogName = strFileName;
	}

	HCSTRR GetFileLogName() const { return m_strFileLogName; }

public:
	HRET WriteLog(HCSTRR log);

protected:
	HSTR newLogName();

	void shiftLog();

private:
	void initlog();
	
	void uninitlog();

	void init();

	void uninit();

	HRET write_log(HCSZ ff, HN ll, HCSZ slog, HCSZ stype = nullptr);

	bool more_day();

private:
	HSTR m_strFileLogName;

	const HSIZE m_settings;

	HSIZE m_len;

	HUN m_nDayIndex;

	// Declare the shift size variable, make it can be configed.
	const HUN m_cnShiftSize;	

	SYS_T m_fd;

	time_t m_logTime;

	pthread_mutex_t* m_mutex;

	struct log_locker_holder {
		log_locker_holder(pthread_mutex_t*);

		~log_locker_holder();

		pthread_mutex_t* pmutex;
	};
};

class HLogManager {
public:
	typedef enum {
		lms_normal,
		lms_warn,
		lms_error,

		lmn_normal,
		lmn_warn,
		lmn_error,

		lm_end
	} lm_index;

	typedef HLog* logs_t[lm_end];

	enum LOGMANAGER_SETTINGS {
		LOG_ONLY_ONE_DEFAULT = 0,
		LOG_WITH_SYSTEM = 1 << 0,
		LOG_SYSTEM_MULTIPLE = 1 << 1,
		LOG_NORMAL_MULTIPLE = 1 << 2,
		LOG_SYSTEM_WITHLOCK = 1 << 3,
		LOG_NORMAL_WITHLOCK = 1 << 4,
		LOG_SYSTEM_SHIFT = 1 << 5,
		LOG_NORMAL_SHIFT = 1 << 6,
	};

public:
	HLogManager () noexcept;

	~ HLogManager ();

public:
	void Setup(HCSTRR strName, HN settings = LOG_ONLY_ONE_DEFAULT, HSIZE nShiftSize = HLog::DEFAULT_LOG_LENGTH);

	void SetLogName(HCSTRR strLogName) {
		m_strName = strLogName;
	}

	HCSTRR GetLogName () const { return m_strName; }	

	HLog* GetLog (lm_index index);

public:
	void WriteLog(lm_index index, HCSTRR log);

	static HSTR GetFlagsStr (lm_index lf);

protected:
	void init ();

	void uninit();


private:
	HN m_settings;

	HSIZE m_nShiftSize = 1024 * 1024 * 100;		// 100M

	HSTR m_strName;

	logs_t m_logs;
};

HRET InitLog(HCSTRR strFilename, HN settings = HLogManager::LOG_ONLY_ONE_DEFAULT,
	HSIZE nShiftSize = HLog::DEFAULT_LOG_LENGTH);

void WriteLog(HLogManager::lm_index fl, HCSZ ff, HN ll, HCSZ fmt, ...);

#define LOG_NORMAL(fmt, args...) WriteLog(HLogManager::lmn_normal, __FILE__, __LINE__, fmt, ##args)
#define LOG_WARNING(fmt, args...) WriteLog(HLogManager::lmn_warn, __FILE__, __LINE__, fmt, ##args)
#define LOG_ERROR(fmt, args...) WriteLog(HLogManager::lmn_error, __FILE__, __LINE__, fmt, ##args)

#define LOG_NS(ss) WriteLog(HLogManager::lmn_normal, __FILE__, __LINE__, ss)
#define LOG_WS(ss) WriteLog(HLogManager::lmn_warn, __FILE__, __LINE__, ss)
#define LOG_ES(ss) WriteLog(HLogManager::lmn_error, __FILE__, __LINE__, ss)

#define HFUN_BEGIN LOG_NORMAL("[%s]: begin...", __FUNCTION__)
#define HFUN_END LOG_NORMAL("[%s]: end...", __FUNCTION__)

#define SLOG_NORMAL(fmt, args...) WriteLog(HLogManager::lms_normal, __FILE__, __LINE__, fmt, ##args)
#define SLOG_WARNING(fmt, args...) WriteLog(HLogManager::lms_warn, __FILE__, __LINE__, fmt, ##args)
#define SLOG_ERROR(fmt, args...) WriteLog(HLogManager::lms_error, __FILE__, __LINE__, fmt, ##args)

#define SLOG_NS(ss) WriteLog(HLogManager::lms_normal, __FILE__, __LINE__, ss)
#define SLOG_WS(ss) WriteLog(HLogManager::lms_warn, __FILE__, __LINE__, ss)
#define SLOG_ES(ss) WriteLog(HLogManager::lms_error, __FILE__, __LINE__, ss)



} // namespace HUIBASE

#endif //__HLOG_H__
