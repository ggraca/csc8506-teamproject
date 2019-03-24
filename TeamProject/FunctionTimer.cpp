#include "FunctionTimer.h"
#include "FunctionTimerManager.h"
#include <time.h>
#include <stdio.h>

FunctionTimer::FunctionTimer()
{
	startTime = static_cast<float>(clock());
}


FunctionTimer::~FunctionTimer()
{
	endTime = static_cast<float>(clock());

	float totalTime = endTime - startTime;

	manager->UpdateFunctionTimer(functionName, totalTime);
}
