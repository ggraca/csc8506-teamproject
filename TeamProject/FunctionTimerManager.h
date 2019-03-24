#pragma once
#include "FunctionTimer.h"
#include <map>
#include <string>

class FunctionTimerManager
{
public:
	FunctionTimerManager();
	~FunctionTimerManager();

protected:
	//Vector of function names to time taken to run function
	std::map<std::string, float> functionTimers;
};

