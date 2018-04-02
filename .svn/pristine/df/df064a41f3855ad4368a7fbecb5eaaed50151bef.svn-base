#pragma once
#include "queue"
#include "DefineInclude.h"
#include "RingBuff.h"
#include <pthread.h>

template<typename T>
class Queue
{
public:
	Queue(void) 
	{
		m_mutex = new pthread_mutex_t();
		pthread_mutex_init(m_mutex, NULL);
	};
	~Queue(void) {};
	void Clear()
	{
		pthread_mutex_lock(m_mutex);
		std::queue<T> empty;
		swap(empty,m_kDataVec);
		pthread_mutex_unlock(m_mutex);
	}
	
	bool Empty()
	{
		return m_kDataVec.empty();
	}
	void PushMsg(T &item)
	{
		pthread_mutex_lock(m_mutex);
		m_kDataVec.push(item);
		pthread_mutex_unlock(m_mutex);
	}
	
	bool PopMsg(T &item)
	{
		pthread_mutex_lock(m_mutex);
		if (0 >= m_kDataVec.size())
		{
			pthread_mutex_unlock(m_mutex);
			return false;
		}
		
		item = m_kDataVec.front();
		m_kDataVec.pop();
		pthread_mutex_unlock(m_mutex);
		return true;
	}
private:
	std::queue<T> m_kDataVec;
	pthread_mutex_t	*m_mutex;
};