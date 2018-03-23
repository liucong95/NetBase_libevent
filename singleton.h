#pragma once

#include "assert.h"
template<typename T>
class Singleton
{
public:

	Singleton(void)
	{
		assert(m_single == NULL);
		m_single = static_cast< T*>(this);
	}

	virtual ~Singleton(void)
	{
		m_single = 0;
	}
	
	static T* GetSingleTonPtr()
	{
		assert(NULL != m_single);
		return m_single;
	}
	
	static T&	GetSingleTon()
	{
		assert(NULL != m_single);
		return *m_single;
	}
	
protected:
	static T*	m_single;
	
};
template<typename T>T* Singleton<T>::m_single = NULL;


