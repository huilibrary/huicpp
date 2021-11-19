


#include "hutil.h"

#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>

namespace HUICPP {

    RET_T hsleep(HUN sec, HUN nMinSec) {

        struct timeval tval = {sec, nMinSec * 1000};

        auto cb = select (0, nullptr, nullptr, nullptr, &tval);

        HASSERT_RETURN(cb == 0, SYS_FAILED);

        HRETURN_OK;

    }
    
/*
    HCSTRR hcmap_get (HCPSR ps, HCSTRR strKey) {

        HPS::const_iterator cit = ps.find(strKey);

        HASSERT_THROW_MSG(cit != ps.end(), "hcmap_get pos out", POS_END);

        return cit->second;

    }


    HUINT hget_rand (HUINT n) {

        struct timeval tv = {0,0};
        (void) gettimeofday(&tv, nullptr);

        (void)srand((unsigned int)tv.tv_usec);

        HUINT res = rand();

        return n != 0 ? res % n : res;

    }


    HDOUBLE GetRang (HUINT n) {
        HUINT m = 2 * n;
        return (HDOUBLE)hget_rand (m * 10) / 10 - ((HDOUBLE)n);
    }


    void hvs2map (HCKVSR ckvs, HCParam& ps) {

        for (size_t i = 0; i < ckvs.size(); ++i) {

            HCKVR kvr = ckvs[i];

            ps[kvr.first] = kvr.second;

        }

    }
    */

}
