#pragma once
#include "SessionHandler.h"

class IoHandler : public SessionHandler
{
public:
	IoHandler(void);
	~IoHandler(void);
	
	virtual int HandleInput();
	virtual int HandleOutput();
	virtual int HandleTimeOut();
	virtual int HandleSingal();
	virtual void HandleClose();
	virtual void ProcessMsg();
};

