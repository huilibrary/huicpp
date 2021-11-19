/*
 * Created: Joseph Hui Tue Dec 27 2016
 * Description: 
  		Wrapper for huicpp socket.
*/


#ifndef __H_HUICPP_SOCKET_H__
#define __H_HUICPP_SOCKET_H__

#include "huicpp.h"
#include "hstr.h"
#include "haddr.h"
#include "htime.h"
#include "todoholder.h"
#include <netinet/tcp.h>
#include <unistd.h>

namespace HUICPP {

	class HSocketException;
    class HSocket;
    class HTcpSocket;
    class HUdpSock;
    class HAddr;
	class HIp4Addr;
	class HTcpClient;

#define RETURN_SOCK_STATUS HASSERT_RETURN(IsGoodSocket(), ERR_STATUS)

#define SOCK_FD_CHECK 	do{ if (not IsGoodSocket()) { \
		throw HSocketException(HStr::Format("It not a good socket. Socket:[%d]", m_sock), __FILE__, __LINE__); \
	} } while(0)

#define THROW_NET_EXCEPTION(ss) throw HSocketException(ss)


class HSocketException : public HException {
public:
	HSocketException(HStr strError, HCSTRR szFile = HSTR(), HN iLineNo = -1)
		: HException(strError, ERR_NO(NETWORK_ERROR), szFile.c_str(), iLineNo) {}
};


class HSocket {
public:
    using socket_t = FD_T;
    constexpr static socket_t SOCKET_DEFA_SOCKET = 0;
    constexpr static HN SOCKET_MAX_CONNECT_TIMEOUT = 3;
	constexpr static HN SOCKET_KEEPALIVE_DEF = 1;
	constexpr static HN SOCKET_KEEPIDLE_DEF = 600;
	constexpr static HN SOCKET_KEEPINTERVAL_DEF = 5;
	constexpr static HN SOCKET_KEEPCOUNT_DEF = 3;
    constexpr static HN SOCKET_DEFAULT_TIMEOUT = 3;

    enum class SOCK_TYPE {
        ST_NORMAL,
        ST_TCP,
        ST_UDP,
    };

public:
    constexpr HSocket (socket_t sock = SOCKET_DEFA_SOCKET) noexcept
	    : m_sock (sock) { }

    HSocket (const HSocket& sock) = delete;

    // move construction
    HSocket (HSocket&& sock) noexcept
    	: m_sock(sock.m_sock) { sock.SetAsInvalid(); }

    virtual ~HSocket() {
	    Close();
    }

    HSocket& operator= (const HSocket& ) = delete;

    HSocket& operator= (HSocket&& sock) {
        m_sock = sock.m_sock;
        sock.SetAsInvalid();
        return *this;
    }

    virtual HRET Init () = 0;    

    HRET Connect (const HAddr& addr) const;

    void Close();

    HRET GetAddrInfo (HAddr& addr) const;

    virtual HRET GetAddrInfo (HSTRR strIp, HNR iPort) const = 0;

    virtual HSTR ToString () const;

    virtual SYS_RET_T Write (HCPTR buf, HSIZE nbyte) const;

    virtual SYS_RET_T WriteAll (HCPTR buf, HSIZE nbyte, 
        HN timeout = SOCKET_DEFAULT_TIMEOUT) const throw (); 

    virtual SYS_RET_T Read (HPTR buf, HSIZE nbyte) const ;

    virtual SYS_RET_T ReadWithTimeOut (HPTR buf, 
        HSIZE nbyte, HN timeout = SOCKET_DEFAULT_TIMEOUT) const throw ();

    SYS_RET_T ReadAllWithTimeout (HPTR buf, HSIZE nbyte, HN timeout = SOCKET_DEFAULT_TIMEOUT) const;

    virtual SYS_RET_T Send (HCPTR buf, HSIZE len, SYS_T flags = 0) const;

    virtual SYS_RET_T SendAll (HCPTR buf, HSIZE len, 
        SYS_T flags = 0, HN timeout = SOCKET_DEFAULT_TIMEOUT) const throw ();

    virtual SYS_RET_T Recv (HPTR buf, HSIZE len, SYS_T flags = 0) const ;

    virtual SYS_RET_T RecvWithTimeOut (HPTR buf, HSIZE len, 
        SYS_T flags = 0, HN timeout = SOCKET_DEFAULT_TIMEOUT) const throw () ;

    bool IsGoodSocket() const { return m_sock != SOCKET_DEFA_SOCKET; }

	socket_t GetSocket () const { return m_sock; }

    void SetAsInvalid () { m_sock = SOCKET_DEFA_SOCKET; }

	void SetSocket (socket_t sock) { m_sock = sock;}

    virtual SOCK_TYPE GetSocketType () const {
	    return SOCK_TYPE::ST_NORMAL;
    }

private:    
    socket_t m_sock;
};


class HTcpSocket : public HSocket {
public:
    using base = HSocket;

public:
    HTcpSocket(socket_t sock = SOCKET_DEFA_SOCKET)
	    : base (sock) {}

    HTcpSocket(const HTcpSocket& sock) = delete;

    HTcpSocket(HTcpSocket&& sock)
        : base(std::move(sock)) {  }

    virtual ~HTcpSocket() { }

    HTcpSocket& operator= (const HTcpSocket& ) = delete;

    HTcpSocket& operator= (HTcpSocket&& sock) {
        base::operator=(std::move(sock));
        sock.SetAsInvalid();
        return *this;
    }

public:
    virtual HRET Init () override;

    HRET ShutDown (SYS_T how = SHUT_RDWR);

    HRET Bind (const HAddr& addr) const ;

    HRET Listen (HUN count) const;

    HRET Accept (HTcpSocket& sock, HAddr& ) const;    

    HRET ConnectWithTimeOut (const HAddr& addr, HN timeout) const;

    HRET SetNonblocked (bool bFlag = true) const;

    bool IsNonblocked () const throw();

	HRET SetKeepAlive (HN keepAlive = SOCKET_KEEPALIVE_DEF, 
		HN keepIdle = SOCKET_KEEPIDLE_DEF, 
		HN keepInterval = SOCKET_KEEPINTERVAL_DEF, 
		HN keepCount = SOCKET_KEEPCOUNT_DEF) const;

	HRET SetReuseAddr () const;

    HRET SetSockOpt (SYS_T optName, HCPTR optVal,
		     socklen_t optLen, SYS_T level = SOL_SOCKET) const;

    HRET GetSockOpt (SYS_T optName, HPTR optVal,
		     socklen_t* optLen, SYS_T level = SOL_SOCKET) const;

    virtual HRET GetAddrInfo (HSTRR strIp, HNR iPort) const override;

 	HIp4Addr GetAddr () const;

    virtual SOCK_TYPE GetSocketType () const override {
	    return SOCK_TYPE::ST_TCP;
    }


};


class HUdpSock : public HSocket {
public:
    using base = HSocket;

public:
    HUdpSock (socket_t fd = SOCKET_DEFA_SOCKET)
        : HSocket(fd) { }

    HUdpSock (const HUdpSock& sock) = delete;

    HUdpSock(HUdpSock&& sock)
        : base(std::move(sock)) {  }

    virtual ~HUdpSock () { }

    HUdpSock& operator= (const HUdpSock& ) = delete;

    HUdpSock& operator= (HUdpSock&& sock) {
        base::operator=(std::move(sock));
        sock.SetAsInvalid();
        return *this;
    }

public:
    virtual HRET Init () override;

    virtual HRET GetAddrInfo (HSTRR strIp, HNR iPort) const override;

    HIp4Addr GetAddr () const;

    virtual SOCK_TYPE GetSocketType () const override {
	    return SOCK_TYPE::ST_UDP;
    }

};


}

#endif //__H_HUICPP_SOCKET_H__



