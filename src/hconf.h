/*
 * Created: Joseph Mar-29-2021
 * Description: 
  		Wrapper for huicpp config. 
*/

#ifndef __H_HUICPP_CONFIG_H__
#define __H_HUICPP_CONFIG_H__

#include "hnormalconf.h"


namespace HUICPP {
    
class HConfig {
public:
	HConfig();

	virtual ~HConfig();

public:
	virtual HRET Init(HCSTRR strConf);

protected:
	virtual HRET load_confs();

public:
	HCSTRR GetConfValue (HCSTRR key) const ;

	HCSTRR GetConfValue (HCSTRR key, HCSTRR defVal) const noexcept;

	void SetConfValue(HCSTRR key, HCSTRR val);

	HN GetConfIntValue (HCSTRR key) const;

	HN GetConfIntValue (HCSTRR key, HN def) const noexcept;

	bool ConfExists(HCSTRR strKey) const;

	HCSTRR GetMapValue (HCSTRR confName, HCSTRR key) const ;

	HCSTRR GetMapValue (HCSTRR confName, HCSTRR key, HCSTRR defVal) const;

	HN GetMapIntValue (HCSTRR confName, HCSTRR key) const;

	HN GetMapIntValue (HCSTRR confName, HCSTRR key, HN def) const;

	bool MapExists(HCSTRR confName, HCSTRR strKey) const; 

	HRET JsonGetAs (HCSTRR confName, HCSTRR key, HSTRR res) const;

	HRET JsonGetAs (HCSTRR confName, HCSTRR key, HNR res) const;

private:
	HNormalConfig m_conf;
	config_map_t m_config_map;
};

}    


#endif // __H_HUICPP_CONFIG_H__









