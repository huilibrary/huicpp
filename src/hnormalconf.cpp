

#include "hnormalconf.h"
#include "hfilestr.h"
#include "adaptation/ada_sys.h"
#include "hstr.h"

namespace HUICPP {


HRET HNormalConfig::LoadConfig (HCSTRR strConfigFileName) {

    HASSERT_RETURN(not strConfigFileName.empty(), INVL_PARA);

    HSTR strConfigContent;
    HFileStr hfs;

    HNOTOK_RETURN(hfs.InOpen(strConfigFileName));
    HNOTOK_RETURN(hfs.ReadToString(strConfigContent));

    HASSERT_RETURN(strConfigContent.length() > MIN_CONTENT_LENGTH, INVL_PARA);

    RET_T ret = ERR_NO(OK);

	do {

	    HVSTR lines;
	    HCSTR key (adapt::hos_br());

        ret = HStr::Split(strConfigContent, key, lines);
        IF_NOTOK(ret) {
            break;
        }

	    HCSTR skey("=");

	    for (HSIZE i = 0; i < lines.size(); ++i) {

            HCSTRR l = lines.at(i);

            if(isNoteLine(l)) {
                continue;
            }

            HVSTR vs;

            ret = HStr::Split(l, skey, vs);
            IF_NOTOK(ret) {
               continue;
            }

            if (vs.size() < 2) {
                continue;
            }

            HSTRR ikey = vs.at(0);
            HStr::Trim(ikey);
            HSTRR ival = vs.at(1);
            HStr::Trim(ival);

            m_conf.SetValue(ikey, ival);

	    }

	} while(0);
	
	return ret;

    HRETURN_OK;

}


void HNormalConfig::Clear() {

    m_conf.clear();

}


HCSTRR HNormalConfig::GetValue(HCSTRR strKey) const {

    return m_conf.GetValue(strKey);

}


HCSTRR HNormalConfig::GetValue(HCSTRR strKey, HCSTRR strDefVal) const noexcept{

    return m_conf.GetValue(strKey, strDefVal);

}


HN HNormalConfig::GetIntValue(HCSTRR strKey) const{

    return m_conf.GetInt(strKey);

}


HN HNormalConfig::GetIntValue(HCSTRR strKey, HN nDefVal) const noexcept{

    return m_conf.GetInt(strKey, nDefVal);

}


bool HNormalConfig::Exists(HCSTRR strKey) const {

    return m_conf.Exists(strKey);

}


void HNormalConfig::SetValue (HCSTRR key, HCSTRR val) noexcept {

    m_conf.SetValue(key, val);

}


bool HNormalConfig::isNoteLine (HCSTRR l) const {

    if (l.length() < 3) { 
        return true;
    }

    if (l.substr(0, 2) == ";;") {
        return true;
    }

    return false;

}


}