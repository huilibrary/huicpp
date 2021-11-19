

#include "network_detail.h"
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/netdevice.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

namespace HUICPP {

namespace detail {


HRET network_detail::GetLocalIps4 (HVSTRR ip4s) {

    return get_local_ips(ip4s, AF_INET);

}



HRET network_detail::GetLocalIps6 (HVSTRR ip6s) {

    return get_local_ips(ip6s, AF_INET6);

}



HRET network_detail::get_local_ips (HVSTRR ips, int domain) {

    struct ifconf ifconf = {0};
    struct ifreq ifr[50] = {0};
    ifconf.ifc_buf = reinterpret_cast<char*>(ifr);
    ifconf.ifc_len = sizeof(ifr);

    int s = socket(domain, SOCK_STREAM, 0);
    HASSERT_RETURN(s > 0, SYS_FAILED);

    HASSERT_RETURN(ioctl(s, SIOCGIFCONF, &ifconf) == 0, SYS_FAILED);

    int ifs = ifconf.ifc_len / sizeof(ifr[0]);
    ips.clear();

    for (int i = 0; i < ifs; i++) {
        char ip[INET_ADDRSTRLEN] = {0};
        struct sockaddr_in *s_in = reinterpret_cast<struct sockaddr_in *>(&ifr[i].ifr_addr);

        HASSERT_RETURN(inet_ntop(domain, &s_in->sin_addr, ip, sizeof(ip)) != nullptr, SYS_FAILED);
        ips.push_back(ip);
    }

    close(s);

    HRETURN_OK;

}


}

}