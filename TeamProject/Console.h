#pragma once
#include <map>
#include <functional>
#include "../GameTechCommon/Debug.h"

class Console
{
public:
	Console();
	~Console();

	void Update();
	void RegisterCommand(string identifier, function<void(vector<string>, void*)> command, void* data);
protected:
	map<string, pair<function<void(vector<string>, void*)>, void*>> commands;
	bool consoleOpen = false;
	string currentCommand = "";

	void HandleCommand();
};

