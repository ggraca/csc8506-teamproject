#include "FunctionTimerManager.h"



FunctionTimerManager::FunctionTimerManager()
{
	FunctionTimer::manager = this;
}


FunctionTimerManager::~FunctionTimerManager()
{
}
