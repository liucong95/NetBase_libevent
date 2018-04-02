#pragma once
#include "SessionHandler.h"

struct	 sockaddr_in;
class Acceptor :public SessionHandler
{
public:
	Acceptor(void);
	~Acceptor(void);
	
	virtual int HandleInput();
	virtual void ProcessMsg();
	virtual void HandleClose();
	void IintListenSock();
private:
	
	int	ListenSockFd;
	sockaddr_in m_kClientAddr;
	socklen_t	m_kSockLen;
};

