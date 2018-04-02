#include <iostream>
#include "SessionManager.h"
#include "IoHandler.h"
#include <signal.h>
#include "Logger.h"
using namespace std;

bool bRunning = true;
void SignalHandle(int i)
{
	bRunning = false;
}

int main(int argc, char *argv[])
{
	::signal(SIGINT, SignalHandle);
	::signal(SIGTERM, SignalHandle);
	::signal(SIGPIPE, SIG_IGN);
	Logger *pkLoger = new Logger("log/");
	pkLoger->StartWriteThread();
	
	char sz[] = "start!";	//Hover mouse over "sz" while debugging to see its contents
	SessionManager *pkMgr = new SessionManager();
	pkMgr->StartEventBase();
	
	for (int i = 0; i < 1005; i++)
	{
		IoHandler *handle = new IoHandler();
		pkMgr->AddSessionToPool((SessionHandler*)handle);
	}
	
	while (bRunning)
	{
		pkMgr->Updata();
	}
	
	pkMgr->Close();
	return 0;
}