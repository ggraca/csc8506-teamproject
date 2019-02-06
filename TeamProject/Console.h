#pragma once
#include <string>
#include <map>
#include <functional>

class Console
{
public:
	Console();
	~Console();

	void Update();
	void RegisterCommand(std::string identifier, std::function<void(std::string)> command);
protected:
	std::map<std::string, std::function<void(std::string)>> commands;
	bool consoleOpen = false;
	std::string consoleCommand = "";

	void HandleCommand();
};

