#pragma once
#include <map>
#include <functional>
#include "Debug.h"

class Console
{
public:
	Console(Console const&) = delete;
	void operator=(Console const&) = delete;

	static Console& GetInstance()
	{
		static Console instance;
		return instance;
	}

	static void Update();
	static void Toggle() { GetInstance().consoleOpen = !(GetInstance().consoleOpen); }
	static void RegisterCommand(string identifier, function<void(vector<string>, void*)> command, void* data);
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
private:
	Console() {};
};

