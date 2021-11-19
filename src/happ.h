/*
 * Created: Joseph Hui Tue Dec 27 2016
 * Description: 
  		Wrapper for huicpp application.
*/


#ifndef __H_HUICPP_APPLICATION_H__
#define __H_HUICPP_APPLICATION_H__

#include "huicpp.h"
#include "hconf.h"
#include "happlock.h"

#include <atomic>
#include <memory>

namespace HUICPP {


class HApp{
public:
	HApp();

	HApp(HN argc, HCSZ argv[] );

	virtual ~ HApp();

public:
	virtual bool Run() = 0;
	
	virtual HRET Init();		
	
	virtual void Stop () { m_bRunning.store(false); }

	bool IsRunning () const { return m_bRunning.load(); }

	void Handle_sig (int sig);

private:
	void anaOpt(int argc, HCSZ argv[]);

	virtual void help() const noexcept;

	void setup_signal() const;

	HRET setup_app_dir();

	HRET setup_config();

	HRET setup_log () ;

	HRET setup_daemon();

	HRET setup_applock();
	

protected:
	virtual void setas_daemon_app();

	virtual void lock_app ();

public:
	static void stop_handle(int sig);
	static void bug_handle(int sig);

public:
	static constexpr HCSZ HOME_PATH = "home_path";
	static constexpr HCSZ EXE_FILEPATH = "exe_filepath";
	static constexpr HCSZ EXE_PATH = "exe_path";
	static constexpr HCSZ EXE_NAME = "exe_name";
	static constexpr HCSZ CONFPATH = "conf_path";
	static constexpr HCSZ CONF_LOG = "log";
	static constexpr HCSZ CONF_PROC = "proc";
	static constexpr HCSZ CONF_DAEMON = "is_daemon";
	static constexpr HCSZ CONF_APP_LOCK = "app_lock";

private:
	bool m_bDaemon;
	bool m_bInited;

	std::atomic<bool> m_bRunning;
	HConfig m_conf;
	std::shared_ptr<HAppLock> m_app_lock;

};

}

#endif //__H_HUICPP_APPLICATION_H__

