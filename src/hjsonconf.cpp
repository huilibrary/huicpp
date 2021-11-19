

#include "hjsonconf.h"
#include "adaptation/ada_json.h"

namespace HUICPP {

HJsonConfig::HJsonConfig() 
    : base (), m_impl(new adapt::jsonconfig_impl()) {

    CHECK_NEWPOINT(m_impl);

}


HJsonConfig::~HJsonConfig() {

    HDELP(m_impl);

}


HRET HJsonConfig::LoadConfig (HCSTRR strConfigFileName) {

    return m_impl->load_json_file(strConfigFileName);

}


void HJsonConfig::Clear() {

    m_impl->clear();

}


HCSTRR HJsonConfig::GetValue(HCSTRR strKey) const {

    return m_impl->get_value(strKey);

}


HCSTRR HJsonConfig::GetValue(HCSTRR strKey, HCSTRR strDefVal) const noexcept {

    return m_impl->get_value(strKey, strDefVal);

}


HN HJsonConfig::GetIntValue(HCSTRR strKey) const {

    return m_impl->get_int_value(strKey);

}


HN HJsonConfig::GetIntValue(HCSTRR strKey, HN nDefVal) const noexcept {

    return m_impl->get_int_value(strKey, nDefVal);

}


HRET HJsonConfig::GetAsStr(HCSTRR key, HSTRR res) const {

    return m_impl->get_as<HSTR>(key, res);
    
}


HRET HJsonConfig::GetAsInt (HCSTRR key, HNR res) const {

    return m_impl->get_as<HN>(key, res);

}


bool HJsonConfig::Exists(HCSTRR strKey) const {

    return m_impl->exists(strKey);

}


void HJsonConfig::SetValue (HCSTRR key, HCSTRR val) noexcept {

    m_impl->set_value(key, val); 

}


}