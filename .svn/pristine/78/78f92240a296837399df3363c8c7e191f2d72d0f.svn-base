#pragma once

#include "stdlib.h"
template<typename T>
class RingBuff
{
public:
	RingBuff() {};
	~RingBuff(void) 
	{
		if(m_data != NULL)
			delete m_data;
	};
	
	bool Init(int iMaxSize)
	{
		m_iTotalSize = iMaxSize;
		m_iSize = 0;
		m_iReadIndex = 0;
		m_iWriteIndex = 0;
		m_data = (T*)malloc(sizeof(T) * m_iTotalSize);
		return true;
	}
	
	bool Write(const T *Data, int Len)
	{
		if (Len + m_iSize >	m_iTotalSize)
		{
			//Log("Size full !!!!");
			return false;
		}
		
		if (m_iReadIndex <= m_iWriteIndex)
		{
			if (m_iWriteIndex + Len > m_iTotalSize)
			{
				int wirteSize = m_iTotalSize - m_iWriteIndex;
				memcpy(m_data + sizeof(T)*m_iWriteIndex, Data, sizeof(T)*wirteSize);
				memcpy(m_data, Data + sizeof(T)*wirteSize,  sizeof(T)*(Len - wirteSize));
			}
			else				
				memcpy(m_data + sizeof(T)*m_iWriteIndex, Data, sizeof(T)*Len);
		}
		else
		{
			memcpy(m_data + sizeof(T)*m_iWriteIndex, Data, sizeof(T)*Len);
		}
		
		m_iSize += Len;
		m_iWriteIndex += Len;
		m_iWriteIndex %= m_iTotalSize;
		return true;
	}
	
	bool Read(T *Data, int Len)
	{
		if (m_iSize <= 0)
		{
			//Log("Size empty!!!!");
			return false;
		}
		
		if (m_iReadIndex >= m_iWriteIndex)
		{
			if (m_iReadIndex + Len > m_iTotalSize)
			{
				int ReadSize = m_iTotalSize - m_iReadIndex;
				memcpy(Data, m_data + sizeof(T)*m_iReadIndex, sizeof(T)*ReadSize);
				memcpy(Data + sizeof(T)*ReadSize, m_data, sizeof(T)*(Len - ReadSize));
			}
			else
				memcpy(Data, m_data + sizeof(T)*m_iReadIndex, sizeof(T)*Len);
		}
		else
		{
			memcpy(Data, m_data + sizeof(T)*m_iReadIndex, sizeof(T)*Len);
		}
		
		m_iSize -= Len;
		m_iReadIndex += Len;
		m_iReadIndex %= m_iTotalSize;
		return true;
	}
	
	bool	Empty()
	{
		return m_iSize == 0 ? true : false;
	}
	
	void	Clear()
	{
		m_iSize = 0;
		m_iReadIndex = 0;
		m_iWriteIndex = 0;
	}
private:
	int		m_iTotalSize;
	int		m_iSize;
	int		m_iReadIndex;
	int		m_iWriteIndex;
	
	T*		m_data;
};

