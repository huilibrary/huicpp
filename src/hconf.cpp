


#include "hconf.h"
#include "hstr.h"
#include "hjsonconf.h"

namespace HUICPP {

HConfig::HConfig() 
	: m_conf(), m_config_map() {


}


HConfig::~HConfig() {

	for(config_map_t::iterator it = m_config_map.begin(); 
		it != m_config_map.end(); ++it) {
		
		config_pointer p = it->second;
		HDELP(p);

	}

	m_config_map.clear();

}


HRET HConfig::Init(HCSTRR strConf) {

	HNOTOK_RETURN(m_conf.LoadConfig(strConf));

	HNOTOK_RETURN(load_confs());

	HRETURN_OK;
}


HRET HConfig::load_confs() {

	if (m_conf.Exists("confs")) {

		HSTR confs_src = m_conf.GetValue("confs");

		HVSTR confs;
		HNOTOK_RETURN(HStr::Split(confs_src, "|", confs));

		for(HVSTR::size_type i = 0; i < confs.size(); ++i) {
			HCSTRR s = confs[i];

			HVSTR cc;
			HNOTOK_RETURN(HStr::Split(s, ":", cc));
			HASSERT_RETURN(cc.size() >= 2, ERR_STATUS);

			HCSTRR type = cc[0];
			HCSTRR name = cc[1];

			HConfigBase* new_conf = nullptr;
			if (type == "normal") {
				new_conf = new HNormalConfig;	
			}

			if (type == "json") {
				new_conf = new HJsonConfig;
			}

			if (new_conf == nullptr) {
				continue;
			}

			HNOTOK_RETURN(new_conf->LoadConfig(HSTR("../conf/") + name));
			m_config_map[name] = new_conf;
		}
	}

	HRETURN_OK;
}


HCSTRR HConfig::GetConfValue (HCSTRR key) const {

	return m_conf.GetValue(key);

}


HCSTRR HConfig::GetConfValue (HCSTRR key, HCSTRR defVal) const noexcept {

	return m_conf.GetValue(key, defVal);

}


void HConfig::SetConfValue(HCSTRR key, HCSTRR val) {

	m_conf.SetValue(key, val);

}


HN HConfig::GetConfIntValue (HCSTRR key) const {

	return m_conf.GetIntValue(key);

}


HN HConfig::GetConfIntValue (HCSTRR key, HN def) const noexcept {

	return m_conf.GetIntValue(key, def);

}


bool HConfig::ConfExists(HCSTRR strKey) const {

	return m_conf.Exists(strKey);

}

#define CCONF_SS config_map_t::const_iterator cfit = m_config_map.find(confName);	\
	HASSERT_THROW(cfit != m_config_map.cend(), INVL_PARA);	\
	const HConfigBase* cconf = cfit->second;	\
	HASSERT_THROW(cconf != nullptr, SRC_ERR)

#define CCONF_JSON_SS config_map_t::const_iterator cfit = m_config_map.find(confName);	\
	HASSERT_RETURN(cfit != m_config_map.cend(), INVL_PARA);	\
	const HConfigBase* cconf = cfit->second;	\
	HASSERT_RETURN(cconf != nullptr, SRC_ERR);	\
	HASSERT_RETURN(cconf->GetType() == HConfigBase::CONFIG_TYPE::CT_JSON, SRC_ERR);	\
	const HJsonConfig* jconf = reinterpret_cast<const HJsonConfig*>(cconf)

HCSTRR HConfig::GetMapValue (HCSTRR confName, HCSTRR key) const {

	CCONF_SS;

	return cconf->GetValue(key);

}


HCSTRR HConfig::GetMapValue (HCSTRR confName, HCSTRR key, HCSTRR defVal) const {

	CCONF_SS;

	return cconf->GetValue(key, defVal);

}


HN HConfig::GetMapIntValue (HCSTRR confName, HCSTRR key) const {

	CCONF_SS;

	return cconf->GetIntValue(key);

}


HN HConfig::GetMapIntValue (HCSTRR confName, HCSTRR key, HN def) const{

	CCONF_SS;

	return cconf->GetIntValue(key, def);

}


bool HConfig::MapExists(HCSTRR confName, HCSTRR strKey) const {

	CCONF_SS;

	return cconf->Exists(strKey);

}


HRET HConfig::JsonGetAs (HCSTRR confName, HCSTRR key, HSTRR res) const {

	CCONF_JSON_SS;
	
	return jconf->GetAsStr(key, res);
	
}


HRET HConfig::JsonGetAs (HCSTRR confName, HCSTRR key, HNR res) const {

	CCONF_JSON_SS;
	
	return jconf->GetAsInt(key, res);

}


#undef CCONF_SS

}

