/*
 * Created: Joseph Hui Tue Dec 27 2016
 * Description: 
  		Wrapper for huicpp socket address.
*/


#ifndef __HADDR_H__
#define __HADDR_H__


#include <type_traits>

#include "huicpp.h"
#include "hsocket.h"

#include <netinet/in.h>

#include "detail/network_detail.h"

namespace HUICPP {

class HSocket;


class HAddr {
public:
	constexpr static HN ADDR_DEFA_PORT = 0;

	enum class ADDR_TYPE {
	    IP_NORMAL,
	    IP_V4,
	    IP_V6
	};
	
public:
    HAddr() = default;

    virtual ~ HAddr () {}

public:
	virtual ADDR_TYPE GetAddrType () const {
		return ADDR_TYPE::IP_NORMAL;
    }

    virtual HRET Bind (const HSocket& _socket) const = 0;

    virtual HRET Connect (const HSocket& _socket) const = 0;

    virtual HRET Accept (const HSocket& server, HSocket& client) = 0;

    virtual HRET GetFromSocket (const HSocket& sock) = 0;
	
    virtual HRET GetAddrInfo (HSTRR strIp, HNR iPort) const = 0;	    

	virtual HSTR ToString () const = 0;

};

class HIp4Addr : public HAddr {	
public:
	using addr_integer_type = HN;
	static constexpr addr_integer_type ADDR_DEFA_VALUE = 0;
	using sockaddr_type = struct sockaddr_in;

public:
	HIp4Addr(addr_integer_type addr_bytes = ADDR_DEFA_VALUE, 
		HN _port = ADDR_DEFA_PORT) noexcept;

	HIp4Addr (HCSTRR strIp, HN _port = ADDR_DEFA_PORT);

	explicit HIp4Addr (HCSTRR strIpPort);

	explicit HIp4Addr (const sockaddr_type& addr);

	virtual ~ HIp4Addr() {}

public:
	ADDR_TYPE GetAddrType () const override{
		return ADDR_TYPE::IP_V4;
	}

	HRET Bind (const HSocket& _sock) const override;

	HRET Connect (const HSocket& _sock) const override;

	HRET Accept (const HSocket& server, HSocket& client) override;

	HRET GetFromSocket (const HSocket& sock) override;

	HRET GetAddrInfo (HSTRR strIp, HNR iPort) const override;

	addr_integer_type GetIpInt () const;

	virtual HSTR ToString () const override;

	static HRET GetLocalIps (HVSTRR ips);


protected:
	virtual HRET init (addr_integer_type addr_bytes = ADDR_DEFA_VALUE, 
		HN _port = ADDR_DEFA_PORT) noexcept;

	virtual HRET init (HCSTRR strIp, HN _port);

	virtual HRET init (const sockaddr_type& addr);

public:
	static HRET SetupNetWithIpString (HCSTRR ip, HN port, sockaddr_type& net); 

	static HRET ParseNetFromString (HCSTRR str, sockaddr_type& net); 

	static HRET GetStringFromNet (const sockaddr_type& addr, HSTRR ip, HNR port);

	
private:
	sockaddr_type m_addr;
};

}

#endif //__HADDR_H__














