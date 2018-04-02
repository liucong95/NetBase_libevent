#include "EventReactor.h"
#include "SessionManager.h"
#include "Include.h"
#include "ThreadHandle.h"

EventReactor::EventReactor(void)
{
}


EventReactor::~EventReactor(void)
{
	if (m_pkBase)
	{
		event_base_free(m_pkBase);
	}
	
	if (m_pkSessionMgr)
	{
		delete(m_pkSessionMgr);
	}
	
	if (m_pkThreadHandle)
	{
		delete(m_pkThreadHandle);
	}
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
	
	m_pkThreadHandle = new ThreadHandle();
	if (NULL == m_pkThreadHandle)
	{
		Log_Err("Logic Error");
	}

	return true;
}

void	EventReactor::StartEventLoop()
{
	if(!m_pkThreadHandle->CreatThread(&EventReactor::EventLoop, (void *)this))
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

	Log("Event_base_loop finish");
	return NULL;
}