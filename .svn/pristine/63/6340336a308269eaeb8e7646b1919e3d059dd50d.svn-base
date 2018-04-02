#include "IoHandler.h"
#include "SessionManager.h"

std::string _Int2String(int x)
{
	char s[5];
	sprintf(s, "%d", x);
	//itoa(x, s, 5);
	return s;
}
IoHandler::IoHandler(void)
{
}


IoHandler::~IoHandler(void)
{
}
void IoHandler::ProcessMsg()
{	
	char	buff[1024];
	if (!m_kMessageQuene.PopMsg(buff))
	{
		Log("sock:%d, Logic Error", GetHandleFd());
		return;
	}
	Log("sock:%d, recive msg2: %s", GetHandleFd(), buff);
	
	std::string sMsg = "this is server repeat!";
	int len = (sMsg.length());
	if (send(GetHandleFd(), (char*)&len, 4, 0) == 0)
		return;
	send(GetHandleFd(), sMsg.c_str(), sMsg.length(), 0);
}

int IoHandler::HandleInput()
{
	int RcvSize = 0;
	SessionMsgType kSessionMsg(OPT_NONE, (SessionHandler *)this);
	int len = recv(GetHandleFd(), (char*)&RcvSize, 4, 0);
	if (len != 4)
	{
		kSessionMsg.kCurrentOperaType = OPT_DISCONNECT;	
		GetSessionMgr()->PushMsgSession(kSessionMsg);
		Log("sock:%d, close Socket1", GetHandleFd());
		return 1;
	}
	
	char buff[1024];
	
	RcvSize = recv(GetHandleFd(), buff, RcvSize, 0);
	if (RcvSize == 0)
	{
		kSessionMsg.kCurrentOperaType = OPT_DISCONNECT;
		Log("sock:%d, close Socket1", GetHandleFd());
	}
	else if (RcvSize < 0)
	{
		kSessionMsg.kCurrentOperaType = OPT_DISCONNECT;
		Log("sock:%d, recvError", GetHandleFd());
	}
	else
	{
		kSessionMsg.kCurrentOperaType = OPT_READ;
		m_kMessageQuene.PushMsg(buff, strlen(buff) + 1);
		Log("sock:%d, recive msg1", GetHandleFd());
	}
			
	GetSessionMgr()->PushMsgSession(kSessionMsg);
	return 1;
}

int IoHandler::HandleOutput()
{
	
}

int IoHandler::HandleTimeOut()
{
	
}

int IoHandler::HandleSingal()
{
	
}

void IoHandler::HandleClose()
{
	if(0 != m_fd)
		close(m_fd);
}
