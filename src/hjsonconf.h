/*
 * Created: Joseph Hui Fri Sep 24 2021
 * Description: 
  		Wrapper for huicpp json config.
*/


#ifndef __H_HUICPP_JSON_CONFIG_H__
#define __H_HUICPP_JSON_CONFIG_H__

#include "hconfigbase.h"

namespace HUICPP {

namespace adapt {
class jsonconfig_impl;
}

class HJsonConfig : public HConfigBase {
public:
    using impl_t = adapt::jsonconfig_impl;
    using impl_pointer = impl_t*;
    using base = HConfigBase;
public:
    HJsonConfig();

    ~HJsonConfig();

public:
    virtual CONFIG_TYPE GetType() const override { return CONFIG_TYPE::CT_JSON; }

    virtual HRET LoadConfig (HCSTRR strConfigFileName) override;

    virtual void Clear() override;

public:
    virtual HCSTRR GetValue(HCSTRR strKey) const override;

    virtual HCSTRR GetValue(HCSTRR strKey, HCSTRR strDefVal) const noexcept override;

    virtual HN GetIntValue(HCSTRR strKey) const override;

    virtual HN GetIntValue(HCSTRR strKey, HN nDefVal) const noexcept override;

    HRET GetAsStr(HCSTRR key, HSTRR res) const;

    HRET GetAsInt (HCSTRR key, HNR res) const;

    virtual bool Exists(HCSTRR strKey) const override;

    virtual void SetValue (HCSTRR key, HCSTRR val) noexcept override;

private:
    impl_pointer m_impl;
};

}

#endif // __H_HUICPP_JSON_CONFIG_H__

