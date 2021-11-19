
#include "hsocket.h"
#include <sys/select.h>

#include <netdb.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <functional>

namespace HUICPP  {


HRET HSocket::Connect (const HAddr& addr) const {

	return addr.Connect(*this);

}


void HSocket::Close() {

	if (m_sock != SOCKET_DEFA_SOCKET) {

		::close(m_sock);

		SetAsInvalid();

	}

}


HRET HSocket::GetAddrInfo (HAddr& addr) const {

	RETURN_SOCK_STATUS;

	return addr.GetFromSocket(*this);

}


HSTR HSocket::ToString () const{

	HSTR strIp;
	HN iPort = 0;

	HIGNORE_RETURN(GetAddrInfo(strIp, iPort));

	return HStr::Format("%s:%d", strIp.c_str(), iPort);

}


SYS_RET_T HSocket::Write (HCPTR buf, HSIZE nbyte) const {

	SYS_RET_T cb = ::write(m_sock, buf, nbyte);

	return cb;
		
}


SYS_RET_T HSocket::WriteAll (HCPTR buf, HSIZE nbyte, HN timeout) const throw () {

	SOCK_FD_CHECK;

	HCSZ pos = reinterpret_cast<HCSZ>(buf);

	HSIZE sent_bytes = 0; 
	SYS_T this_send = 0;
	while (sent_bytes < nbyte) {
		//do {
			if (timeout > 0) {
				fd_set write_fds;
				FD_ZERO(&write_fds);
				FD_SET(m_sock, &write_fds);

				timeval tval = {0,0};
				tval.tv_sec = timeout;
				tval.tv_usec = 0;

				SYS_RET_T cb = select(m_sock +1, NULL, &write_fds, NULL, &tval);
				if (cb == 0) {
					THROW_NET_EXCEPTION("write all time out");
				} else if (cb < 0) {
					THROW_NET_EXCEPTION("select failed");
				}
			}

			this_send = ::write(GetSocket(), pos, nbyte - sent_bytes);
            //} while(this_send >= 0);

		if (this_send <= 0) {
			return this_send;
		}

		sent_bytes += this_send;
		pos += this_send;
	}

	return sent_bytes;
}


SYS_RET_T HSocket::Read (HPTR buf, HSIZE nbyte) const {

	SYS_RET_T cb = ::read(GetSocket(), buf, nbyte);

	return cb;

}


SYS_RET_T HSocket::ReadWithTimeOut (HPTR buf, HSIZE nbyte, HN timeout) const throw (){
	
	SOCK_FD_CHECK;

	if (timeout > 0) {
	
		fd_set read_fds;
		FD_ZERO(&read_fds);
		FD_SET(GetSocket(), &read_fds);

		timeval tval = {0,0};
		tval.tv_sec = timeout;
		tval.tv_usec = 0;

		SYS_RET_T cb = select(GetSocket() +1, &read_fds, NULL, NULL, &tval);
		if (cb == 0) {
			THROW_NET_EXCEPTION("read all time out");
		} else if (cb < 0) {
			THROW_NET_EXCEPTION("select failed");
		}

	}

    SYS_RET_T ret = ::read(GetSocket(), buf, nbyte);

	return ret;
	
}


SYS_RET_T HSocket::ReadAllWithTimeout (HPTR buf, HSIZE nbyte, HN timeout) const {

	SYS_RET_T cb = 0, ret = 0;
	HSZ pos = reinterpret_cast<HSZ>(buf);
	HSIZE temp_nbyte = nbyte;

	do {

		try {

			cb = ReadWithTimeOut(pos, temp_nbyte, timeout);

			if (cb > 0) {
				ret += cb;
				pos += cb;
				temp_nbyte -= cb;
			} 
			
		} catch (const HException& ex) {
			break;
		}

	} while(cb > 0);

	return ret;

}


SYS_RET_T HSocket::Send (HCPTR buf, HSIZE len, SYS_T flags) const {
		
	SYS_RET_T cb = ::send(GetSocket(), buf, len, flags);

	return cb;

}


SYS_RET_T HSocket::SendAll (HCPTR buf, HSIZE len, SYS_T flags, HN timeout) const throw () {

	SOCK_FD_CHECK;

	HCSZ pos = static_cast<HCSZ>(buf);

	HSIZE sent_bytes = 0;
	HN this_send = 0;

	while (sent_bytes < len) {
		do {
			if (timeout > 0) {
				fd_set write_fds;
				FD_ZERO(&write_fds);
				FD_SET(m_sock, &write_fds);

				timeval tval = {timeout,0};

				SYS_RET_T cb = select(GetSocket() + 1, nullptr, &write_fds, nullptr, &tval);
				if (cb == 0) {
					THROW_NET_EXCEPTION("sendall all time out");
				} else if (cb < 0) {
					THROW_NET_EXCEPTION("select failed");
				}
			}

			this_send = ::send(GetSocket(), pos, len - sent_bytes, flags);
		} while((this_send < 0) && (errno == EINTR) );

		if (this_send <= 0) {
			return sent_bytes;
		}

		sent_bytes += this_send;
		pos += this_send;
	}

	return sent_bytes;

}


SYS_RET_T HSocket::Recv (HPTR buf, HSIZE len, SYS_T flags) const {

	SYS_RET_T cb = ::recv(GetSocket(), buf, len, flags);

	return cb;

}


SYS_RET_T HSocket::RecvWithTimeOut (HPTR buf, 
	HSIZE len, SYS_T flags , HN timeout ) const throw () {

	SOCK_FD_CHECK;

	if (timeout > 0) {
	
		fd_set read_fds;
		FD_ZERO(&read_fds);
		FD_SET(GetSocket(), &read_fds);

		timeval tval = {timeout, 0};

		SYS_RET_T cb = select(GetSocket() +1, &read_fds, nullptr, nullptr, &tval);
		if (cb == 0) {
			THROW_NET_EXCEPTION("read all time out");
		} else if (cb < 0) {
			THROW_NET_EXCEPTION("select failed");
		}
		
	}

	SYS_RET_T ret = ::recv(GetSocket(), buf, len, flags);

	return ret;
}


HRET HTcpSocket::Init ()  {
	
	// close if socket is established.
	Close();

	socket_t sock = socket(AF_INET, SOCK_STREAM, 0);
	HASSERT_RETURN(sock > 0, SYS_FAILED);
	SetSocket(sock);

	HRETURN_OK;

}


HRET HTcpSocket::ShutDown (SYS_T how) {

	SYS_RET_T cb = ::shutdown (GetSocket(), how);
	HASSERT_RETURN(cb == 0, SYS_FAILED);

	HRETURN_OK;	
}


HRET HTcpSocket::Bind (const HAddr& addr) const  {

	return addr.Bind(*this);

}


HRET HTcpSocket::Listen (HUN _count) const{

	SYS_RET_T cb = ::listen(GetSocket(), _count);
	HASSERT_RETURN(cb == 0, SYS_FAILED);

	HRETURN_OK;
}


HRET HTcpSocket::Accept (HTcpSocket& sock, HAddr& addr)  const{

	return addr.Accept(*this, sock);
	
}


HRET HTcpSocket::ConnectWithTimeOut (const HAddr& addr, HN timeout) const {

	RETURN_SOCK_STATUS;

	if (timeout <= 0) { timeout = SOCKET_MAX_CONNECT_TIMEOUT; }

	// setup SetNonblocked holder; 
	bool bHasSet = false;
	auto dof = [this, &bHasSet]() {
		if (not this->IsNonblocked()) {
			bHasSet = true;
			HIGNORE_RETURN(this->SetNonblocked());
		}
	};

	auto udf = [this, bHasSet]() {
		if (bHasSet) {
			HIGNORE_RETURN(this->SetNonblocked(false));
		}
	};

	typedef TodoHolder<decltype(dof), decltype(udf)> set_block_holder;
	set_block_holder todo_holder(dof, udf);
	
	// try to connect
	while (NOT_OK(addr.Connect(*this))) {

		// errno == EINPROGRESS

		fd_set wfds;
		struct timeval tval = {timeout, 0};

		FD_ZERO(&wfds);
		FD_SET(GetSocket(), &wfds);

		HN cb = 0;

		while((cb = select(GetSocket() + 1, nullptr, &wfds, nullptr, &tval)) < 0) {
			
			HASSERT_RETURN(errno == EINTR, SYS_FAILED);

			continue;

		}

		if (cb == 0) {
			HRETURN(TIME_OUT);
		} else {
			
			// almost finished...
			SYS_T iError = -1;
			HSIZE slen = sizeof(iError);
			getsockopt(GetSocket(), SOL_SOCKET, SO_ERROR, &iError, (socklen_t*)&slen);
			if (iError != 0) {
				throw HSocketException(HStr::Format("ConnectWithTimeOut error: %s", strerror(iError)));
			}
			
		}
		
	}

	HRETURN_OK;

}


HRET HTcpSocket::SetNonblocked (bool bFlag) const{

	RETURN_SOCK_STATUS;

	SYS_T flags = 0;
	HASSERT_RETURN((flags = fcntl(GetSocket(), F_GETFL, 0)) >= 0, SYS_FAILED);

	if (bFlag and flags & O_NONBLOCK) {
		HRETURN_OK;
	}

	if (bFlag and not (flags & O_NONBLOCK)) {
		HRETURN_OK;
	}

	if (bFlag) {
		// set to non-block mode
		HASSERT_RETURN(fcntl(GetSocket(), F_SETFL, flags | O_NONBLOCK) >= 0, SYS_FAILED);
	} else {
		HASSERT_RETURN(fcntl(GetSocket(), F_SETFL, flags | ~O_NONBLOCK) > 0, SYS_FAILED);
	}

	HRETURN_OK;
}


bool HTcpSocket::IsNonblocked () const throw() {

	SYS_T flags = 0;
	HASSERT_THROW((flags = fcntl(GetSocket(), F_GETFL, 0)) >= 0, SYS_FAILED);

	return flags & O_NONBLOCK;
}


/**
	keepAlive: trun on keepalive
	keepIdle: try to network plumbing if no data transafe in 600's
	keepInterval: timeval 
	keepCount: times for network plumbing
**/
HRET HTcpSocket::SetKeepAlive (HN keepAlive, HN keepIdle, HN keepInterval, HN keepCount) const {

	RETURN_SOCK_STATUS;

	if (keepAlive >= 0) {
		HNOTOK_RETURN(SetSockOpt(SO_KEEPALIVE, 
			static_cast<HPTR>(&keepAlive), sizeof(keepAlive), SOL_SOCKET));
	}

	if (keepAlive >= 0) {
    	HNOTOK_RETURN(SetSockOpt(TCP_KEEPIDLE, 
			static_cast<HPTR>(&keepIdle), sizeof(keepIdle), SOL_TCP));
	}

	if (keepAlive >= 0) {
    	HNOTOK_RETURN(SetSockOpt(TCP_KEEPINTVL, 
			static_cast<HPTR>(&keepInterval), sizeof(keepInterval), SOL_TCP));
	}

	if (keepAlive >= 0) {
    	HNOTOK_RETURN(SetSockOpt(TCP_KEEPCNT, 
			static_cast<HPTR>(&keepCount), sizeof(keepCount), SOL_TCP));
	}

	HRETURN_OK;
}


HRET HTcpSocket::SetReuseAddr () const {

	int rea = 1;
	return SetSockOpt(SO_REUSEADDR, 
		static_cast<void*>(&rea), sizeof(rea), SOL_SOCKET);

}


HRET HTcpSocket::SetSockOpt (SYS_T optName, HCPTR optVal,
		     socklen_t optLen, SYS_T level ) const {

	SYS_RET_T cb = setsockopt(GetSocket(), level, optName, optVal, optLen);
	HASSERT_RETURN(cb == 0, SYS_FAILED);

	HRETURN_OK;
}


HRET HTcpSocket::GetSockOpt (SYS_T optName, HPTR optVal, socklen_t* optLen, SYS_T level) const {

	SYS_RET_T cb = getsockopt(GetSocket(), level, optName, optVal, optLen);
	HASSERT_RETURN(cb == 0, SYS_FAILED);

	HRETURN_OK;	
}


HRET HTcpSocket::GetAddrInfo (HSTRR strIp, HNR iPort) const {

	HIp4Addr addr;
	HNOTOK_RETURN(base::GetAddrInfo(addr));

	return addr.GetAddrInfo(strIp, iPort);
}


HIp4Addr HTcpSocket::GetAddr () const {

	HIp4Addr ret;
	HIGNORE_RETURN(base::GetAddrInfo(ret));

	return ret;
}


HRET HUdpSock::Init() {

	// close if socket is established.
	Close();

	socket_t sock = socket(AF_INET, SOCK_DGRAM, 0);
	HASSERT_RETURN(sock > 0, SYS_FAILED);
	SetSocket(sock);

	HRETURN_OK;

}


HRET HUdpSock::GetAddrInfo(HSTRR strIp, HNR iPort) const {

    HIp4Addr addr = GetAddr();

    return addr.GetAddrInfo(strIp, iPort);

}


HIp4Addr HUdpSock::GetAddr () const {

    HIp4Addr ret ;

    HIGNORE_RETURN(ret.GetFromSocket(*this));

    return ret;

}


}


