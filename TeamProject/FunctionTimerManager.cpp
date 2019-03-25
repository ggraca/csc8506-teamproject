#include "FunctionTimerManager.h"
#include "FunctionTimer.h"


FunctionTimerManager::FunctionTimerManager()
{
	FunctionTimer::manager = this;
}


FunctionTimerManager::~FunctionTimerManager()
{
}
