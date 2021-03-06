#include "Acceptor.h"
#include "SessionManager.h"

Acceptor::Acceptor(void)
{
}


Acceptor::~Acceptor(void)
{
}

void Acceptor::IintListenSock()
{
	sockaddr_in serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr("192.168.11.128");
	serverAddr.sin_port = htons(10086);

	
	m_pkLister = evconnlistener_new_bind(GetEventBase(), 
		&Acceptor::listener_cb, 
		(void *)this, 
		LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE,
		-1,
		(sockaddr*) &serverAddr,
		sizeof(sockaddr));
	if (NULL == m_pkLister)
	{
		Log_Err("evconnlistener_new_bind Error");
		return;
	}
	
	Log("start listen");
}

void Acceptor:: ProcessMsg()
{
	
}
int Acceptor::HandleInput()
{
	SessionManager *pkSessionMgr = GetSessionMgr();
	if(NULL == pkSessionMgr)
	{
		Log_Err("Logic Error");
		return -1;
	}
		
	int sock = accept(m_fd, (sockaddr *)&m_kClientAddr, &m_kSockLen);
	if (sock <= 0)
	{
		Log_Err("Logic Error");
		return 0;
	}

	SessionHandler *pkSessionHandle = pkSessionMgr->GetSessionFromPool();
	if (NULL == pkSessionHandle)
	{
		Log_Err("Logic Error");
		return -1;
	}

	pkSessionHandle->Reset();
	pkSessionHandle->SetHandleFd(sock);
	pkSessionHandle->SetSessionMgr(pkSessionMgr);
	pkSessionHandle->SetEventReactor(GetEventReactor());
	pkSessionHandle->RegisterIoEvent(EV_ET | EV_READ | EV_PERSIST);
	
	SessionMsgType kSessionMsg;
	kSessionMsg.kCurrentOperaType = OPT_CONNECT;
	pkSessionMgr->PushMsgSession(SessionMsgType(OPT_CONNECT, pkSessionHandle));
	
	Log("Accept new Connection");
	return 0;

}

void Acceptor::HandleClose()
{
	if (NULL != m_pkLister)
		evconnlistener_free(m_pkLister);
}

void Acceptor::listener_cb(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *sa, int socklen, void *user_data)  
{
	Acceptor *pkAccepet = (Acceptor *)user_data;
	if (NULL == pkAccepet)
	{
		Log_Err("listener_cb Error");
		return;
	}

	SessionManager *pkSessionMgr = pkAccepet->GetSessionMgr();
	if (NULL == pkSessionMgr)
	{
		Log_Err("Logic Error");
		return;
	}

	SessionHandler *pkSessionHandle = pkSessionMgr->GetSessionFromPool();
	if (NULL == pkSessionHandle)
	{
		Log_Err("Logic Error");
		return;
	}

	pkSessionHandle->Reset();
	pkSessionHandle->SetHandleFd(fd);
	pkSessionHandle->SetSessionMgr(pkSessionMgr);
	pkSessionHandle->SetEventReactor(pkAccepet->GetEventReactor());
	pkSessionHandle->RegisterIoEvent(EV_ET | EV_READ | EV_PERSIST);

	SessionMsgType kSessionMsg;
	kSessionMsg.kCurrentOperaType = OPT_CONNECT;
	pkSessionMgr->PushMsgSession(SessionMsgType(OPT_CONNECT, pkSessionHandle));

	Log("Accept new Connection");
	return;
} 