#pragma once


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

class ThreadHandle
{
public:

	ThreadHandle(void)
	{
		m_hEventHandle = CreateEvent(NULL, FALSE, TRUE, NULL);
	}

	~ThreadHandle(void)
	{
	}

	void Lock()
	{
		WaitForSingleObject(m_hEventHandle, INFINITE);
	}

	void UnLock()
	{
		SetEvent(m_hEventHandle);
	}

	static DWORD WINAPI RunThread(LPVOID lpParam)  
	{  
		ThreadParam * pkThreadParam = (ThreadParam *)lpParam;
		if (NULL == pkThreadParam->Fun)
			return NULL;

		(*pkThreadParam->Fun)(lpParam);
		return 0;
	}


	bool	CreatThread(ThreadFun fun, void *arg)
	{
		m_hHandle = CreateThread(NULL, 0, &ThreadHandle::RunThread, (LPVOID)new ThreadParam(fun, this, arg), 0, &m_iThreadID);
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
	HANDLE m_hEventHandle;
};


#elseclass ThreadHandle
{
public:

	ThreadHandle(void)
	{
		m_mutex = new pthread_mutex_t();
		pthread_mutex_init(m_mutex, NULL);
	}

	~ThreadHandle(void)
	{
		if (NULL != m_mutex)
			pthread_mutex_destroy(m_mutex);
	}
	
	void Lock()
	{
		pthread_mutex_lock(m_mutex);
	}
	
	void UnLock()
	{
		pthread_mutex_unlock(m_mutex);
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
	pthread_mutex_t	*m_mutex;
};
#endif // _WIN32
