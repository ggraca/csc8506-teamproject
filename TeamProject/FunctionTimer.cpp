#include "FunctionTimer.h"
#include "FunctionTimerManager.h"
#include <time.h>
#include <stdio.h>

FunctionTimerManager* FunctionTimer::manager = nullptr;

FunctionTimer::FunctionTimer()
{
	
}

FunctionTimer::FunctionTimer(std::string FunctionName) {
	functionName = FunctionName;
	startTime = static_cast<float>(clock());
}


FunctionTimer::~FunctionTimer()
{
	endTime = static_cast<float>(clock());

	float totalTime = endTime - startTime;

	manager->UpdateFunctionTimer(functionName, totalTime);
}
