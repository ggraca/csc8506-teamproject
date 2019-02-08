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
	void Toggle() { consoleOpen = !consoleOpen; }
	void RegisterCommand(string identifier, function<void(vector<string>, void*)> command, void* data);
protected:
	map<string, pair<function<void(vector<string>, void*)>, void*>> commands;
	vector<string> previousCommands;
	bool consoleOpen = false;
	bool acceptingInput = false;
	string currentCommand = "";
  
	int indicatorTimer = 0;
	int previousCommandIndex = 0;

	void HandleCommand();
	void AddToPreviousCommandList();
};

