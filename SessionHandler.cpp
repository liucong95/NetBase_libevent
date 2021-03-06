#include "SessionHandler.h"

SessionHandler::SessionHandler(void)
{
	for (int i = ET_NONE; i < ET_MAX; i++)
		m_kEventType[i] = NULL;	
}


SessionHandler::~SessionHandler(void)
{
	Reset();
}

void SessionHandler::Reset()
{
	HandleClose();
	m_fd = 0;
	m_kMessageQuene.Clear();
	m_pkEventReactor = NULL;
	
	FreeEvent();
}

void SessionHandler::FreeEvent()
{
	for (int i = ET_NONE; i < ET_MAX; i++)
	{	
		if (NULL != m_kEventType[i])
		{
			event_del(m_kEventType[i]);
		}
		m_kEventType[i] = NULL;	
	}
}

event_base *SessionHandler::GetEventBase()
{
	if (NULL == m_pkEventReactor)
	{
		Log_Err("Logic Error");
		return NULL;
	}
	
	return m_pkEventReactor->GetEventBase();
}

void SessionHandler::ProcessOpreation_cb(int fd, short what, void *arg)
{
	SessionHandler *pkSessionHandle = (SessionHandler*) arg;
	if (NULL == pkSessionHandle)
	{
		Log_Err("Logic Error");
		return ;
	}
	
	if (pkSessionHandle->GetHandleFd() != fd)
	{
		Log_Err("Logic Error");
		return ;
	}
	
	int err = 0;
	if (what & EV_READ)
	{
		if ((err = pkSessionHandle->HandleInput()) == -1)
			goto ProcessErr;
	}
	
	if (what & EV_WRITE)
	{
		pkSessionHandle->HandleOutput();
		if ((err = pkSessionHandle->HandleOutput()) == -1)
			goto ProcessErr;
	}
	
	if (what & EV_SIGNAL)
	{
		if ((err = pkSessionHandle->HandleSingal()) == -1)
			goto ProcessErr;
	}
	
	if(what & EV_TIMEOUT)
	{
		if ((err = pkSessionHandle->HandleTimeOut()) == -1)
			goto ProcessErr;
	}

ProcessErr:
	if (err == -1)
		pkSessionHandle->HandleClose();
}

int	SessionHandler::RegisterIoEvent(short what)
{
	if (NULL == m_kEventType[ET_RW])
		m_kEventType[ET_RW] = event_new(GetEventBase(), GetHandleFd(), what, reinterpret_cast<event_callback_fn>(&SessionHandler::ProcessOpreation_cb), (void*)this);
	else
		event_assign(m_kEventType[ET_RW], GetEventBase(), GetHandleFd(), what, reinterpret_cast<event_callback_fn>(&SessionHandler::ProcessOpreation_cb), (void*)this);			
	
	return event_add(m_kEventType[ET_RW], 0);
}

int SessionHandler::RegisterTimeEvent(timeval &tv)
{
	if (NULL == m_kEventType[ET_TIME])
		m_kEventType[ET_TIME] = event_new(GetEventBase(), GetHandleFd(), EV_TIMEOUT, reinterpret_cast<event_callback_fn>(&SessionHandler::ProcessOpreation_cb), (void*)this);
	else
		event_assign(m_kEventType[ET_TIME], GetEventBase(), GetHandleFd(), EV_TIMEOUT, reinterpret_cast<event_callback_fn>(&SessionHandler::ProcessOpreation_cb), (void*)this);			
	
	return event_add(m_kEventType[ET_TIME], &tv);
}

int SessionHandler::RegisterSignalEvent()
{
	return 0;
}