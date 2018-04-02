#pragma once

#ifdef _WIN32
#include "windows.h"
class Lock
{
public:
	Lock()
	{
		m_hEventHandle = CreateEvent(NULL, FALSE, TRUE, NULL);
	};
	~Lock(){};

	void lock()
	{
		WaitForSingleObject(m_hEventHandle, INFINITE);
	}

	void unlock()
	{
		SetEvent(m_hEventHandle);
	}
private:
	HANDLE m_hEventHandle;
};
#else
#include "pthread.h"
class Lock
{
public:
	Lock()
	{
		m_mutex = new pthread_mutex_t();
		pthread_mutex_init(m_mutex, NULL);
	};
	~Lock()
	{
		if (NULL != m_mutex)
			pthread_mutex_destroy(m_mutex);
	};

	void lock()
	{
		pthread_mutex_lock(m_mutex);
	}

	void unlock()
	{
		pthread_mutex_unlock(m_mutex);
	}
private:
	pthread_mutex_t	*m_mutex;
};
#endif
