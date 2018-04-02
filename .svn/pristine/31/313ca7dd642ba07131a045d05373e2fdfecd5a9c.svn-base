#include "SessionManager.h"
#include "SessionHandler.h"

SessionManager::SessionManager(void)
{
}


SessionManager::~SessionManager(void)
{
}
long long iSleepSecu = 0;
void SessionManager::Updata()
{
	//Ò»Ãë
	iSleepSecu = 500000;
	
	SessionMsgType kSessionMsgType;
	while (m_kMassageSessionQueue.PopMsg(kSessionMsgType))
	{
		if (kSessionMsgType.kCurrentOperaType == OPT_NONE)
			break;
		
		SessionHandler *Handeler = kSessionMsgType.pkSessionHandler;
		
		if (kSessionMsgType.kCurrentOperaType == OPT_CONNECT)
		{
			InsertActiveSessionMap(Handeler);
			Log("sock:%d,insert Session", Handeler->GetHandleFd());
		}
		else if (kSessionMsgType.kCurrentOperaType == OPT_DISCONNECT)
		{
			Log("sock:%d,close2", Handeler->GetHandleFd());
			DeleteSessionFromActive(Handeler);
			Handeler->Reset();
			AddSessionToPool(Handeler);			
		}
		else if (kSessionMsgType.kCurrentOperaType == OPT_READ)
		{
			Handeler->ProcessMsg();
		}
		
		iSleepSecu /= 10;
		
	}
	
	
	usleep(iSleepSecu);
}

void SessionManager::Close()
{
	m_kAcceptor->HandleClose();
	
	SESSION_MAP_ITER itBegin = m_kActiveSessionMap.begin(), itEnd = m_kActiveSessionMap.end();
	for (; itBegin != itEnd; itBegin++)
	{
		itBegin->second->HandleClose();
	}
}

void SessionManager::RegisterListenPort()
{
	if (NULL != m_kAcceptor)
	{
		Log_Err("Logic Error");
		return;
	}
	
	m_kAcceptor = new Acceptor();
	m_kAcceptor->SetSessionMgr(this);
	m_kAcceptor->SetEventReactor(m_kEventReactor);
	m_kAcceptor->IintListenSock();

}
void SessionManager::StartEventBase()
{
	m_kEventReactor = new EventReactor();
	m_kEventReactor->InitEventBase(this);
	
	RegisterListenPort();
	
	m_kEventReactor->StartEventLoop();	
}
bool	SessionManager::InsertActiveSessionMap(SessionHandler * pHandler)
{
	if (!HaveInActiveSessionMap(pHandler))
		return false;
	
	m_kActiveSessionMap.insert(std::make_pair(pHandler->GetHandleFd(), pHandler));	
	Log("Accept new Connection2");
	return true;
}

bool	SessionManager::HaveInActiveSessionMap(SessionHandler * pHandler)
{
	SESSION_MAP_CONSTITER itFind = m_kActiveSessionMap.find(pHandler->GetHandleFd());
	if (itFind != m_kActiveSessionMap.end())
	{
		Log_Err("Logic Error");
		return false;
	}
	
	return true;
}

void	SessionManager::DeleteSessionFromActive(SessionHandler * pHandler)
{
	SESSION_MAP_ITER itFind = m_kActiveSessionMap.find(pHandler->GetHandleFd());
	if (itFind == m_kActiveSessionMap.end())
	{
		Log_Err("Logic Error");
		return;
	}
	
	m_kActiveSessionMap.erase(itFind);
}
	
void	SessionManager::AddSessionToPool(SessionHandler *pHandler)
{
	m_kSessionPool.PushMsg(pHandler);
}

SessionHandler * SessionManager::GetSessionFromPool()
{
	SessionHandler * s = NULL;
	if (!m_kSessionPool.PopMsg(s))
	{
		Log_Err("Logic Error");
		return NULL;
	}
	
	return s;
}
	
void	SessionManager::PushMsgSession(SessionMsgType pHandler)
{
	m_kMassageSessionQueue.PushMsg(pHandler);
}

SessionMsgType SessionManager::PopMsgSession()
{
	SessionMsgType s;
	m_kMassageSessionQueue.PopMsg(s);
	return s;
}
