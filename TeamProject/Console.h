#pragma once
#include <string>

class Console
{
public:
	Console();
	~Console();

	void Update();

protected:
	bool consoleOpen = false;
	std::string consoleCommand = "";

	void HandleCommand();
};

