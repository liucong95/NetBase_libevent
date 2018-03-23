#pragma once
#include "MessageQuene.h"
#include "Acceptor.h"
#include "EventReactor.h"
#include "map"

class SessionHandler;

struct SessionMsgType
{
	SessionMsgType()
		: kCurrentOperaType(OPT_NONE)
		, pkSessionHandler(NULL) {};
	SessionMsgType(OperatType type, SessionHandler* handle)
		: kCurrentOperaType(type)
		, pkSessionHandler(handle) {};
	OperatType	kCurrentOperaType;
	SessionHandler *pkSessionHandler;
};

class SessionManager
{
public:
	SessionManager(void);
	~SessionManager(void);
	
	typedef std::map<int, SessionHandler *> SESSION_MAP;
	typedef SESSION_MAP::iterator	SESSION_MAP_ITER;
	typedef SESSION_MAP::const_iterator	SESSION_MAP_CONSTITER;
	
	bool	InsertActiveSessionMap(SessionHandler *);
	bool	HaveInActiveSessionMap(SessionHandler *);
	void	DeleteSessionFromActive(SessionHandler *);
	
	void	AddSessionToPool(SessionHandler *);
	SessionHandler * GetSessionFromPool();
	
	void	PushMsgSession(SessionMsgType);
	SessionMsgType PopMsgSession();
	
	void Updata();
	void Close();
	
	void StartEventBase();
	void RegisterListenPort();
private:
	//处于激活状态的连接
	SESSION_MAP m_kActiveSessionMap;
	
	//连接池， 来一个连接就分配一个
	Queue<SessionHandler *> m_kSessionPool;
	
	//需要处理的连接，I/O事件，连接等
	Queue<SessionMsgType> m_kMassageSessionQueue;
	
	//listen
	Acceptor * m_kAcceptor;
	
	//event_base
	EventReactor * m_kEventReactor;
};