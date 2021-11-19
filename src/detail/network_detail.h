/*
 * Created: Joseph Hui Sat Sep 18 2021
 * Description: 
  		Wrapper for huicpp address function detail.
*/

#ifndef __H_HUICPP_NETWORK_DETAIL_H__
#define __H_HUICPP_NETWORK_DETAIL_H__


#include "../huicpp.h"

namespace HUICPP {

namespace detail {

class network_detail {
protected:
    network_detail() { }
    virtual ~network_detail () { }

public:
    static HRET GetLocalIps4 (HVSTRR ip4s);

    static HRET GetLocalIps6 (HVSTRR ip6s);


private:
    static HRET get_local_ips (HVSTRR ips, int domain);

};

}

}


#endif //__H_HUICPP_NETWORK_DETAIL_H__
