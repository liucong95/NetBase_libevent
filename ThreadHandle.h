#pragma once
#include "lock.h"

typedef void *(*ThreadFun)(void *);

#ifdef _WIN32
#include "windows.h"
	
struct ThreadParam 
{
	ThreadParam(ThreadFun f, void *ar)
		: Fun(f)
		, arg(ar) {}
	;
	ThreadFun	Fun;
	void *arg;
};

class ThreadHandle : public Lock
{
public:

	ThreadHandle(void)
	{
	}

	~ThreadHandle(void)
	{
	}

	static DWORD WINAPI RunThread(LPVOID lpParam)  
	{  
		ThreadParam * pkThreadParam = (ThreadParam *)lpParam;
		if (NULL == pkThreadParam->Fun)
			return NULL;

		(*pkThreadParam->Fun)(pkThreadParam->arg);
		return 0;
	}


	bool	CreatThread(ThreadFun fun, void *arg)
	{
		m_hHandle = CreateThread(NULL, 0, &ThreadHandle::RunThread, (LPVOID)new ThreadParam(fun, arg), 0, &m_iThreadID);
		if (NULL == m_hHandle)
			return false;
		return true;
	}
	void	JoinThread()
	{
		CloseHandle(m_hHandle);
	}

private:
	DWORD m_iThreadID;
	HANDLE m_hHandle;
};


#elseclass ThreadHandle  : public Lock
{
public:

	ThreadHandle(void)
	{
	}

	~ThreadHandle(void)
	{
	}
	
	bool	CreatThread(ThreadFun fun, void *arg)
	{
		int err = pthread_create((pthread_t *)&pkThread, 0, fun, arg); 
		if (err < 0)
			return false;
		return true;
	}
	
	void	JoinThread()
	{
		pthread_join(pkThread, 0);
		close(pkThread);
	}

private:	unsigned long int pkThread;
};
#endif // _WIN32
