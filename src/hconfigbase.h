/*
 * Created: Joseph Mar-29-2021
 * Description: 
  		Wrapper for huicpp config base. 
*/


#ifndef __H_HUICPP_CONFIG_BASE_H__
#define __H_HUICPP_CONFIG_BASE_H__

#include "huicpp.h"

namespace HUICPP {

class HConfigBase {
public:
    enum class CONFIG_TYPE {
        CT_BASE,
        CT_NORMAL,
        CT_JSON
    };

public:
    HConfigBase() { }

    virtual ~ HConfigBase () { }

public:
    virtual CONFIG_TYPE GetType() const { return CONFIG_TYPE::CT_BASE; }

    HRET ReloadConfig (HCSTRR strConfigFileName);
    
    virtual HRET LoadConfig (HCSTRR strConfigFileName) = 0;

    virtual void Clear() = 0;

public:
    virtual HCSTRR GetValue(HCSTRR strKey) const = 0;

    virtual HCSTRR GetValue(HCSTRR strKey, HCSTRR strDefVal) const noexcept = 0;

    virtual HN GetIntValue(HCSTRR strKey) const = 0;

    virtual HN GetIntValue(HCSTRR strKey, HN nDefVal) const noexcept = 0;

    virtual bool Exists(HCSTRR strKey) const = 0;

    virtual void SetValue (HCSTRR key, HCSTRR val) noexcept = 0;
};

using config_pointer = HConfigBase*;
using config_map_t = std::map<HSTR, config_pointer>;

}


#endif // __H_HUICPP_CONFIG_BASE_H__

