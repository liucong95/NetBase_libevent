#pragma once

#ifdef _WIN32
#include "winsock2.h"
#include "windows.h"
#include<ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#include <event2/event.h>
#include "iostream"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "Logger.h"


#define Log(fmt, ...) Logger::GetSingleTonPtr()->format(1, __FILE__,__LINE__, fmt, ##__VA_ARGS__)

#define Log_Warn(fmt, ...) Logger::GetSingleTonPtr()->format(2, __FILE__,__LINE__, fmt, ##__VA_ARGS__)

#define Log_Err(fmt, ...) Logger::GetSingleTonPtr()->format(3, __FILE__,__LINE__, fmt, ##__VA_ARGS__)

#ifdef _WIN32
#define SLEEP(x) Sleep(x)
#else
#define SLEEP(x) usleep(x)
#endif // _WIN32

enum	OperatType
{
	OPT_NONE = 0,
	OPT_CONNECT = 1,
	OPT_DISCONNECT = 2,
	OPT_READ = 3,
	OPT_WRITE = 4,
	OPT_TIMEOUT = 5,
	OPT_SINGLE = 6,
	OPT_ERROR = 7,
	OPT_MAX,
};

enum	EventType
{
	ET_NONE = 0,
	ET_RW = 1,
	ET_TIME = 2,
	ET_SIGNAL = 3,
	ET_MAX,
};