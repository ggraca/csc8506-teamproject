#pragma once
#include "FunctionTimerManager.h"
#include <string>
#include <time.h>
#include <stdio.h>

class FunctionTimer
{
friend class FunctionTimerManager;

public:
	FunctionTimer() {};
	FunctionTimer(std::string FunctionName) {
		functionName = FunctionName;
		startTime = static_cast<float>(clock());
	};

	~FunctionTimer() {
		endTime = static_cast<float>(clock());

		float totalTime = endTime - startTime;

		manager->UpdateFunctionTimer(functionName, totalTime);
	};

protected:
	static FunctionTimerManager* manager;

	std::string functionName;
	float startTime;
	float endTime;
};
