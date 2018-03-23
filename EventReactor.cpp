#include "EventReactor.h"
#include "SessionManager.h"
#include "DefineInclude.h"

EventReactor::EventReactor(void)
{
}


EventReactor::~EventReactor(void)
{
}

bool	EventReactor::InitEventBase(SessionManager *pkSessionMgr)
{
	if (NULL == pkSessionMgr)
	{
		Log_Err("Logic Error");
		return false;
	}
	
	m_pkBase = event_base_new();
	if (NULL == m_pkBase)
	{
		Log_Err("Logic Error");
		return false;
	}
	
	m_pkSessionMgr = pkSessionMgr;
}

void	EventReactor::StartEventLoop()
{
	int err = pthread_create((pthread_t *)&m_kThread, 0, &EventReactor::EventLoop, (void*)this);
	if (err < 0)
	{
		Log_Err("thread create error");
		return;
	}
}

void*	EventReactor::EventLoop(void *arg)
{
	EventReactor *pkEventReactor = (EventReactor*)arg;
	if (NULL == pkEventReactor)
	{
		Log_Err("Logic Error");
		return NULL;
	}
	
	event_base_loop(pkEventReactor->GetEventBase(), 0);
	return NULL;
}