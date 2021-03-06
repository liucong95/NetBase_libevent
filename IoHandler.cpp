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
	memcpy(buff, (char*)&len, sizeof(int));
	memcpy(buff + sizeof(int), sMsg.c_str(), len);
	send(GetHandleFd(), buff, len + sizeof(int), 0);
}

int IoHandler::HandleInput()
{
	int RcvSize = 0;
	SessionMsgType kSessionMsg(OPT_NONE, (SessionHandler *)this);
	int len = recv(GetHandleFd(), (char*)&RcvSize, 4, 0);
	if (len != 4)
	{
		if (len == 0)
		{
			kSessionMsg.kCurrentOperaType = OPT_DISCONNECT;			
			Log("sock:%d, close Socket1", GetHandleFd());
			FreeEvent();
			GetSessionMgr()->PushMsgSession(kSessionMsg);
			return 1;
		}
		//可以不用关闭连接，直接丢弃该连接的未处理消息，等待下一次客户端正确的数据
		else
		{
			m_kMessageQuene.Clear();
			Log("sock:%d, dataHead Error", GetHandleFd());
			return 1;
		}
		
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
		FreeEvent();
	}
	else
	{
		kSessionMsg.kCurrentOperaType = OPT_READ;
		m_kMessageQuene.PushMsg(buff, RcvSize+1);
		Log("sock:%d, recive msg1", GetHandleFd());
	}
			
	GetSessionMgr()->PushMsgSession(kSessionMsg);
	return 1;
}

int IoHandler::HandleOutput()
{
	return -1;
}

int IoHandler::HandleTimeOut()
{
	return -1;
}

int IoHandler::HandleSingal()
{
	return -1;
}

void IoHandler::HandleClose()
{
	if(0 != m_fd)
#ifdef _WIN32
		closesocket(m_fd);
#else
		close(m_fd);
#endif // _WIN32
}
