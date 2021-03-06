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
	static void listener_cb(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *sa, int socklen, void *user_data);
private:
	
	struct evconnlistener * m_pkLister;
	sockaddr_in m_kClientAddr;
	socklen_t	m_kSockLen;
};

