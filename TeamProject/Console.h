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
	void RegisterCommand(string identifier, function<void(vector<string>)> command);
	void Toggle() { consoleOpen = !consoleOpen; }
protected:
	map<string, function<void(vector<string>)>> commands;
	bool consoleOpen = false;
	string currentCommand = "";

	void HandleCommand();
};

