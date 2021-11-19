/*
 * Created: Joseph Hui Tue Dec 27 2016
 * Description: 
  		A couple utility.
*/

#ifndef __H_HUICPP_UTIL_H__
#define __H_HUICPP_UTIL_H__


#include "huicpp.h"


namespace HUICPP {

    RET_T hsleep(HUN sec, HUN nMinSec = 0);

/*
    HCSTRR hcmap_get (HCPSR ps, HCSTRR strKey);


    HUINT hget_rand (HUINT n = 0);

    inline void hstr_uvs (HCSTRR str, HMEMR mem) {
        std::copy(str.begin(), str.end(), std::back_inserter(mem));
    }


    inline HSTR huvs_str(HCMEMR mem) {

        HSTR str;

        std::copy (mem.begin(), mem.end(), std::back_inserter(str));

        return str;
    }


    inline void hstr_vs (HCSTRR str, HBUFR buf) {

        std::copy (str.begin(), str.end(), std::back_inserter(buf));

    }

    HDOUBLE GetRang (HUINT n);

    void hvs2map (HCKVSR ckvs, HCParam& ps);
    */
}

#endif
