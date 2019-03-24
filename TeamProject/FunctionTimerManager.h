#pragma once
#include <map>
#include <string>

class FunctionTimerManager
{
public:
	FunctionTimerManager();
	~FunctionTimerManager();

	void UpdateFunctionTimer(std::string FunctionName, float timeTaken) { functionTimers.insert(std::make_pair(FunctionName, timeTaken)); }

	std::map<std::string, float>& GetFunctionTimerMap() { return functionTimers; }

protected:
	//Map of function names to time taken to run function
	std::map<std::string, float> functionTimers;
};

