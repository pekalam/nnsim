#pragma once
#include "NetworkContext.h"
#include "CommandExecutor.h"

class AppContext
{
public:
	AppContext();
	~AppContext();
	NetworkContext networkContext;
	CommandExecutor commandEngine;
};

