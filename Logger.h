#pragma once
#include "iostream"
#include "RingBuff.h"
#include "stdio.h"
#include "time.h"
#include "ThreadHandle.h"
#include "singleton.h"

enum LogType
{
	INFO = 1,
	WARN = 2,
	ERROR = 3,
};

#pragma pack(1)
struct PackHeader
{
	PackHeader(): type(0), size(0) {};
	~PackHeader() {};
	void Reset() {type = 0;size = 0;};
	unsigned short type : 8;
	unsigned int size : 16;
};
#pragma pack()

class FileHandler
{
public:
	~FileHandler(){close();};
	
	bool InitFile(const char *Path)
	{
		char filePath_info[1024];		
		sprintf(filePath_info, "%s%s", Path, "info.log");
		OpenFile(&m_pkInfoFile, filePath_info);
		
		char filePath_warn[1024];	
		sprintf(filePath_warn, "%s%s", Path, "warn.log");
		OpenFile(&m_pkWarnFile, filePath_warn);
		
		char filePath_erro[1024];
		sprintf(filePath_erro, "%s%s", Path, "erro.log");
		OpenFile(&m_pkErroFile, filePath_erro);
		
		return true;
	}
	
	int Write(int type, char* str, int len)
	{
		size_t size = 0;
		switch ((LogType)type)
		{
		case INFO:
			size = fwrite(str, 1, len, m_pkInfoFile);
			break;
		case WARN:
			size = fwrite(str, 1, len, m_pkWarnFile);
			break;
		case ERROR:
			size = fwrite(str, 1, len, m_pkErroFile);
			break;			
		}
		
		printf("%s", str);
		return size ;
	}
	
	void flush()
	{
		if (m_pkInfoFile)
			fflush(m_pkInfoFile);
		if (m_pkWarnFile)
			fflush(m_pkWarnFile);
		if (m_pkErroFile)
			fflush(m_pkErroFile);
	}
	
	void close()
	{
		if (m_pkInfoFile)
			fclose(m_pkInfoFile);
		if (m_pkWarnFile)
			fclose(m_pkWarnFile);
		if (m_pkErroFile)
			fclose(m_pkErroFile);
	}
private:
	
	void OpenFile(FILE **handle, const char *path)
	{
		assert(NULL == *handle);
		*handle = fopen(path, "ab+");
		assert(NULL != *handle);
	}
private:
	FILE*	m_pkInfoFile;
	FILE*	m_pkWarnFile;
	FILE*	m_pkErroFile;
};

class Logger : public Singleton<Logger>
{
public:
	Logger() {};
	Logger(const char *filePath)
	{
		m_bRun = true;
		
		m_kFileHandle = new FileHandler();
		m_kFileHandle->InitFile(filePath);
		
		ThreadMgr = new ThreadHandle();
		m_kLogQueue.Init(65535);
	}

	~Logger(void)
	{
		m_bRun = false;
				
		if(NULL != m_kFileHandle)
			delete(m_kFileHandle);
		
	}
	
	bool Active(){return m_bRun;};
	void SetActive(bool run) {m_bRun = run;};
	
	const char *GetFileName(const char *file)
	{
		int len = strlen(file);
		while (len-- > 0)
		{
			if (file[len] == '/')
				return &file[len+1];
		}
		return file;
	}
	
	char *GetTime()
	{
		time_t timer = time(NULL);
		
		struct tm *colock = localtime(&timer);
		
		sprintf(time_str, "%04d-%02d-%02d %02d:%02d:%02d",
		        colock->tm_year + 1900, colock->tm_mon + 1, colock->tm_mday, colock->tm_hour, colock->tm_min, colock->tm_sec);

	}
	
	void format(int type ,const char * file, int line, const char* fmt, ...)
	{
		GetTime();
		
		std::string sType = type == INFO ? "INFO" : (type == WARN ? "WARN" : "ERROR");
		
		int n = snprintf(dest_str, 1023, "%s %s %s:%d: ", time_str, sType.c_str(), GetFileName(file), line);
		va_list ap;
		va_start(ap, fmt);
		n += ::vsnprintf(dest_str + n, sizeof(dest_str) - n - 1, fmt, ap);
		va_end(ap);
		
		dest_str[n] = '\r';
		dest_str[n+1] = '\n';
		dest_str[n+2] = '\0';
		
		m_kHeader.type = type;
		m_kHeader.size = strlen(dest_str) + 1;
		
		PushLog((const char*)&m_kHeader, sizeof(PackHeader));
		PushLog(dest_str, m_kHeader.size);
	}
	
	void StartWriteThread()
	{
		if (!ThreadMgr->CreatThread(&Logger::WriteThread, (void*)this))
		{
			printf("thread error\n");
		}
	}
	
	static void *WriteThread(void * arg)
	{
		Logger *pkLogger = (Logger*)arg;		
		if (NULL == pkLogger)
			return NULL;
		if (NULL == pkLogger->m_kFileHandle)
			return NULL;
		
		char msgBuff[1024];
		while (pkLogger->Active())
		{			
			PackHeader kHeader;
			pkLogger->PopLog((char*)&kHeader, sizeof(kHeader));
			
			if (kHeader.size <= 0)
			{	
				pkLogger->m_kFileHandle->flush();
				usleep(100000);
				continue;
			}
			
			pkLogger->PopLog(msgBuff, kHeader.size);
			
			pkLogger->m_kFileHandle->Write(kHeader.type, msgBuff, kHeader.size);
			
		}
	}
	
	void PushLog(const char *src, int len)
	{
		ThreadMgr->Lock();
		m_kLogQueue.Write(src, len);
		ThreadMgr->UnLock();
	}
	
	void PopLog(char *des, int len)
	{
		ThreadMgr->Lock();
		m_kLogQueue.Read(des, len);
		ThreadMgr->UnLock();
	}
private:
	char dest_str[1024]; 
	
	char time_str[100];
private:
	bool	m_bRun;
	
	RingBuff<char>	m_kLogQueue;
	
	FileHandler		*m_kFileHandle;
	
	ThreadHandle	*ThreadMgr;
	
	PackHeader m_kHeader;
};