

#include <netdb.h>
#include "haddr.h"

#include <arpa/inet.h>

namespace HUICPP {


HIp4Addr::HIp4Addr(addr_integer_type addr_bytes, HN _port) noexcept{

	HIGNORE_RETURN(init(addr_bytes, _port));

}

HIp4Addr::HIp4Addr (HCSTRR strIp, HN _port) {

	HIGNORE_RETURN(init(strIp, _port));
	
}

HIp4Addr::HIp4Addr (HCSTRR strIpPort){

	ZERO_MEM(&m_addr, sizeof(m_addr));
	HIGNORE_RETURN(ParseNetFromString(strIpPort, m_addr));
	
}

HIp4Addr::HIp4Addr (const struct sockaddr_in& addr) {

	HIGNORE_RETURN(init(addr));

}


HRET HIp4Addr::Bind (const HSocket& _sock) const {

	SYS_RET_T cb = ::bind(_sock.GetSocket(), (const struct sockaddr*)&m_addr, sizeof(m_addr));
	HASSERT_RETURN(cb == 0, SYS_FAILED);

	HRETURN_OK;
	
}

HRET HIp4Addr::Connect (const HSocket& _sock) const {

	SYS_RET_T cb = ::connect(_sock.GetSocket(), (const struct sockaddr*)&m_addr, sizeof(m_addr));
	HASSERT_RETURN(cb == 0, SYS_FAILED);

	HRETURN_OK;

}

HRET HIp4Addr::Accept (const HSocket& server, HSocket& client) {

	socklen_t len = sizeof(m_addr);
	SYS_T cb = ::accept(server.GetSocket(), (struct sockaddr*)&m_addr, &len);
	HASSERT_RETURN(cb > 0, SYS_FAILED);

	client.SetSocket(cb);

	HRETURN_OK;
}

HRET HIp4Addr::GetFromSocket (const HSocket& sock) {

	socklen_t len = sizeof(m_addr);
	SYS_RET_T cb = getpeername(sock.GetSocket(), (struct sockaddr*)&m_addr, &len);
	HASSERT_RETURN(cb == 0, SYS_FAILED);

	HRETURN_OK;
}

HRET HIp4Addr::GetAddrInfo (HSTRR strIp, HNR iPort) const {

	strIp = inet_ntoa(m_addr.sin_addr);
	iPort = ntohs(m_addr.sin_port);

	HRETURN_OK;

}


HIp4Addr::addr_integer_type HIp4Addr::GetIpInt () const {

	return inet_lnaof(m_addr.sin_addr);

}


HSTR HIp4Addr::ToString () const {

	HSTR strIp;
	HN port = ADDR_DEFA_PORT;
	
	IF_NOTOK(GetAddrInfo(strIp, port)) {
		return "";
	}

	return HStr::Format("%s:%d", strIp.c_str(), port);
}


HRET HIp4Addr::GetLocalIps (HVSTRR ips) {

	return detail::network_detail::GetLocalIps4(ips);

}


HRET HIp4Addr::init (addr_integer_type addr_bytes, HN _port) noexcept {

	m_addr.sin_family = AF_INET;
	m_addr.sin_port = htons(_port);
	memcpy(&m_addr.sin_addr.s_addr, &addr_bytes, sizeof(m_addr.sin_addr.s_addr));

	HRETURN_OK;
}


HRET HIp4Addr::init (HCSTRR strIp, HN _port) {

	return SetupNetWithIpString(strIp, _port, m_addr);

}


HRET HIp4Addr::init (const sockaddr_type& addr) {

	memcpy(&m_addr, &addr, sizeof(m_addr));
	HRETURN_OK;

}


HRET HIp4Addr::SetupNetWithIpString (HCSTRR ip, HN port, sockaddr_type& net) {

	net.sin_family = AF_INET;
	net.sin_port = htons(port);
	net.sin_addr.s_addr = inet_addr(ip.c_str());
	HRETURN_OK;
}


HRET HIp4Addr::ParseNetFromString (HCSTRR str, sockaddr_type& net) {

	memset(&net, 0, sizeof(net));
	
	// 127.0.0.1:809
	HCSTR ip_split = ":";
	HSIZE pos = str.find(ip_split);
	if (pos == HStr::npos || pos == str.length() -1) {
		HRETURN(INVL_PARA);
	}

	auto is_ip_str = [] (HCSTRR ip) ->bool {
		return ip.find_first_not_of("1234567890.") == HSTR::npos;
	};

	HSTR ssip = str.substr(0, pos);
	if (!is_ip_str(ssip)) {
		HRETURN(INVL_PARA);
	}

	HN iPort = HStr::ToN(str.substr(pos+1));

	return SetupNetWithIpString(ssip, iPort, net);
	
}


HRET HIp4Addr::GetStringFromNet (const sockaddr_type& addr, HSTRR ip, HNR port) {
	
	port = addr.sin_port;
	ip = inet_ntoa(addr.sin_addr);

	HRETURN_OK;
}

}

