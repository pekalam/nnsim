#pragma once
#include "CommandType.h"

class CommandHandler
{
public:
	virtual void handleCommand(CommandType cmd) = 0;
protected:
	CommandHandler(){}
	
};

