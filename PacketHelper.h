#pragma once

#include "RingBuff.h"
#include "DefineInclude.h"

class PacketHelper
{
public:

	PacketHelper(void)
	{
		m_kMessageData.Init(655);
	}

	~PacketHelper(void)
	{
	}
	
	bool Empty()
	{
		return m_kMessageData.Empty();
	}
	
	void Clear()
	{
		m_kMessageData.Clear();
	}
	
	void PushMsg(const char *data, int size, short type = 0)
	{
		m_kHeader.Reset();
		m_kHeader.type = type;
		m_kHeader.size = size;
		
		if (!m_kMessageData.Write((char*)&m_kHeader, sizeof(PackHeader)))
		{
			Log_Err("Logic Error");
			return;
		}
		
		if (!m_kMessageData.Write(data, size))
		{
			Log_Err("Logic Error");
			return;
		}
	}
	
	bool PopMsg(char *data)
	{
		if (m_kMessageData.Empty())
			return false;
		
		PackHeader header;
		if (!m_kMessageData.Read((char*)&header, sizeof(PackHeader)))
		{
			Log_Err("Logic Error");
			return false;
		}
		
		if (!m_kMessageData.Read(data, header.size))
		{
			Log_Err("Logic Error");
			return false;;
		}
		return true;
	}
	
private:
	RingBuff<char>	m_kMessageData;
	PackHeader		m_kHeader;
};

