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
#include <event2/listener.h>
#include "iostream"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "Logger.h"
#include "Define.h"
