#pragma once
class Console
{
public:
	Console();
	~Console();
protected:
	bool consoleOpen = false;
	std::string consoleCommand = "";

	void HandleCommand();
};

