#pragma once
#include "FunctionTimer.h"
#include <map>
#include <string>

class FunctionTimerManager
{
public:
	FunctionTimerManager();
	~FunctionTimerManager();

	void UpdateFunctionTimer(std::string FunctionName, float timeTaken) { functionTimers.insert(std::make_pair(FunctionName, timeTaken)); }

protected:
	//Map of function names to time taken to run function
	std::map<std::string, float> functionTimers;
};

