#pragma once

struct event_base;

class ThreadHandle;
class SessionManager;
class EventReactor
{
public:
	EventReactor(void);
	~EventReactor(void);
	
	event_base* GetEventBase() {return m_pkBase;};
	SessionManager* GetSessionMgr() {return m_pkSessionMgr;};
	
	bool	InitEventBase(SessionManager *pkSessionMgr);
	void	StartEventLoop();
private:
	static void*	EventLoop(void *arg);
	
private:
	event_base	*m_pkBase;
	SessionManager	*m_pkSessionMgr;
	
	ThreadHandle*	m_pkThreadHandle;
};

