
#include "happ.h"
#include "hlog.h"
#include <getopt.h>
#include "hstr.h"
#include "hfilename.h"

#include <signal.h>
#include <unistd.h>


namespace HUICPP {

static HApp* gapp = nullptr;

static HCSZ s_cmdopts = "c:d:h:l:p";
static const struct  option s_opts[] = {
    { "config", 1, nullptr, 'c' },
    { "deamon", 1, nullptr, 'd' },
    { "help", 1, nullptr, 'h' },
    { "log", 1, nullptr, 'l' },
    { "proc", 1, nullptr, 'p' },
    { nullptr, 0, nullptr, 0 }
};


HApp::HApp()
    : m_bDaemon(true), m_bInited(false), m_bRunning(true),
        m_conf(), m_app_lock(nullptr) {

}


HApp::HApp(HN argc, HCSZ argv[])
    : m_bDaemon(true), m_bInited(false), m_bRunning(true),
        m_conf(), m_app_lock(nullptr) { 

    anaOpt(argc, argv);

}


HApp::~HApp () {

}


void HApp::Handle_sig(int sig) {

    SLOG_ERROR("===========================signal [%d]======================", sig);

}


HRET HApp::Init(){

	setup_signal();

	gapp = this;

    HNOTOK_MSG_RETURN(setup_app_dir(), "app setup direction failed");

    HNOTOK_MSG_RETURN(setup_config(), "app setup configuration failed");
	
	HNOTOK_MSG_RETURN(setup_log(), "app setup log failed");

    SLOG_NORMAL("Home[%s] Conf[%s] Log[%s]", m_conf.GetConfValue(HOME_PATH).c_str(),
    	m_conf.GetConfValue(CONFPATH).c_str(), m_conf.GetConfValue(CONF_LOG).c_str());

    HNOTOK_MSG_RETURN(setup_daemon(), "app setup as daemon failed");
    
	// process lock
    HNOTOK_MSG_RETURN(setup_applock(), "app lock failed");	

    HRETURN_OK;
}


void HApp::anaOpt(int argc, HCSZ argv[]){

    HCSZ cp = nullptr, program = *argv, exename = nullptr;
    m_conf.SetConfValue(EXE_FILEPATH, program);

    cp = strrchr(program, HFileName::root_elem);
    if (cp != nullptr) {
        exename = cp + 1;
    }

    m_conf.SetConfValue(EXE_NAME, exename);
    m_conf.SetConfValue(EXE_PATH, HSTR(program, cp));

    int cb = 0;
    while ((cb = getopt_long( argc, const_cast<char**>(argv), 
        s_cmdopts, s_opts, nullptr)) != -1 ){
        switch (cb) {
            case 'c':
                m_conf.SetConfValue(CONFPATH, optarg);
                break;
            case 'd':
                m_bDaemon = true;
                break;
            case 'h':
                help();
                exit(EXIT_SUCCESS);
                break;
            case 'l':
                m_conf.SetConfValue(CONF_LOG, optarg);
                break;
            case 'p':
                m_conf.SetConfValue(CONF_PROC, optarg);
                break;            
            default:               
                break;
        }
    }
}


void HApp::help() const noexcept {

    printf("Huicpp Frameword\n");
    printf("Please override this function to print help message\n");

}


void HApp::setup_signal() const {

    signal(SIGTERM, stop_handle);
    signal(SIGINT, stop_handle);
    signal(SIGSEGV, bug_handle);

    signal(SIGILL, stop_handle);
    signal(SIGFPE, stop_handle);
    signal(SIGABRT, stop_handle);

}


HRET HApp::setup_app_dir() {

    HASSERT_RETURN(m_conf.ConfExists(EXE_PATH), SRC_ERR);

    HCSTRR strExePath = m_conf.GetConfValue(EXE_PATH);
    HDirection dir(strExePath);

    HSTR home_dir = dir.GetParent().GetPath();
    m_conf.SetConfValue(HOME_PATH, home_dir);

    HRETURN_OK;
}


HRET HApp::setup_config() {

    if (not m_conf.ConfExists(CONFPATH)) {
        HDirection home_dir(m_conf.GetConfValue(HOME_PATH));
        HDirection conf_dir = home_dir.Append("conf");
        m_conf.SetConfValue(CONFPATH, conf_dir.GetPath());
    }

    IF_NOTOK(m_conf.Init(m_conf.GetConfValue(CONFPATH))) {
        printf("Load Config File Fialed. Process exit.");
		exit(-1);
    }

    HRETURN_OK;
}


HRET HApp::setup_log () {

    /*
        get log file name.
    */
    HSTR strLogFileName;

	if (m_conf.ConfExists(CONF_LOG)) {
        // if log path is in config, we take from it.
		strLogFileName = m_conf.GetConfValue(CONF_LOG);	    
	} else {
        // make log path.
        HDirection home_dir(m_conf.GetConfValue(HOME_PATH));
        HDirection log_dir = home_dir.Append("log");

        // make log file name.
        HSTR strExeName = m_conf.GetConfValue(EXE_NAME);
        strExeName = HFileName::path_only_filename(strExeName);
        strExeName.append(".log");
        HIGNORE_RETURN(HStr::Lower(strExeName));
        HFileName fnLog = log_dir.GetDicFile(strExeName);
		m_conf.SetConfValue(CONF_LOG, fnLog.GetPath());
	}

	if( strLogFileName.empty() ){      
	    strLogFileName = m_conf.GetConfValue(CONF_LOG);
	}
    
    return InitLog(strLogFileName.c_str() , HLogManager::LOG_WITH_SYSTEM);

}


HRET HApp::setup_daemon() {

    HSTR dea_str;
	if (m_conf.ConfExists(CONF_DAEMON)) {
		dea_str = m_conf.GetConfValue(CONF_DAEMON);
	   	HStr::Trim(dea_str);
	    SLOG_NORMAL("whether daemon instance [%s]", dea_str.c_str());
	    if( dea_str == "1" ){
	        SLOG_NS("set as daemon");
	        setas_daemon_app();
	    }
	}

    HRETURN_OK;
}


HRET HApp::setup_applock() {

    HSTR applock_str;
	if (m_conf.ConfExists(CONF_APP_LOCK)) {
		applock_str = m_conf.GetConfValue(CONF_APP_LOCK, "1");
	   	HStr::Trim(applock_str);
	    SLOG_NORMAL("whether lock instance [%s]", applock_str.c_str());
	    if( applock_str == "1" ){
	        lock_app();
	    }
	}

    HRETURN_OK;
}


void HApp::setas_daemon_app(){
        
    if (fork() > 0) {	
        exit(0);    
    }    

    setsid();   

    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGHUP, SIG_IGN); 

    if (fork() > 0) {
        exit(0);
    }
    
    umask(0);
    LOG_NORMAL("daemon process no:[%d]", getpid());

}


void HApp::lock_app () {

    HSTR strLockFileName = HSTR(".") + HFileName::path_only_filename(m_conf.GetConfValue(EXE_NAME));
    
    m_app_lock = std::make_shared<HAppLock>(strLockFileName);
    CHECK_NEWPOINT(m_app_lock);

    HIGNORE_RETURN(m_app_lock->LockApp());
    
}


void HApp::stop_handle(int sig) {

	if (gapp) {
        SLOG_WS("stop main process");
		gapp->Stop();
	}

}


void HApp::bug_handle (int sig) {

    if(gapp) {
        gapp->Handle_sig(sig);
    }

}


}

