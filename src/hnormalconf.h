/*
 * Created: Joseph Mar-29-2021
 * Description: 
  		Wrapper for huicpp normal config. 
*/


#ifndef __H_HUICPP_NORMAL_CONFIG_H__
#define __H_HUICPP_NORMAL_CONFIG_H__

#include "hconfigbase.h"
#include "hmstr.h"

namespace HUICPP {

class HNormalConfig : public HConfigBase {
public:
    using base = HConfigBase;
    using cfg_map_t = HMstr;

    static constexpr HN MIN_CONTENT_LENGTH = 10;

public:
    HNormalConfig() { }

    virtual ~ HNormalConfig () { }

public: 
    virtual CONFIG_TYPE GetType() const override { return CONFIG_TYPE::CT_NORMAL; }

    virtual HRET LoadConfig (HCSTRR strConfigFileName) override;

    virtual void Clear() override;

public:
    virtual HCSTRR GetValue(HCSTRR strKey) const override;

    virtual HCSTRR GetValue(HCSTRR strKey, HCSTRR strDefVal) const noexcept override;

    virtual HN GetIntValue(HCSTRR strKey) const override;

    virtual HN GetIntValue(HCSTRR strKey, HN nDefVal) const noexcept override;

    virtual bool Exists(HCSTRR strKey) const override;

    virtual void SetValue (HCSTRR key, HCSTRR val) noexcept override;

private:
    bool isNoteLine (HCSTRR l) const;

private:
    cfg_map_t m_conf;
};

}


#endif // __H_HUICPP_NORMAL_CONFIG_H__

