#pragma once

#include <string>

class FunctionTimer
{
friend class FunctionTimerManager;

public:
	FunctionTimer();
	~FunctionTimer();
protected:
	static FunctionTimerManager* manager;

	std::string functionName;
	float startTime;
	float endTime;
};

