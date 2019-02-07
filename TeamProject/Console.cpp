#include "Console.h"
#include "utils.h"

using namespace NCL;

void TestCommand(vector<string> commandparams, void* data) {
	cout << "Command found with name: " + commandparams[0] << endl;
	cout << "Parameters are: " << endl;;
	for (int i = 1; i < commandparams.size(); i++)
	{
		cout << commandparams[i] << endl;
	}
}

Console::Console()
{
	RegisterCommand("testcommand", TestCommand, nullptr);
}


Console::~Console()
{
}

void Console::HandleCommand() {
	function<void(vector<string>, void*)> functionCall;
	vector<string> parsedCommand = split_string(currentCommand, ' ');

	auto foundCommand = commands.find(parsedCommand[0]);
	AddToPreviousCommandList();
	previousCommandIndex = previousCommands.size();
	currentCommand = "";

	if (foundCommand == commands.end()) {
		cout << __FUNCTION__ << " was called with invalid command!" << endl;
		return;
	}

	functionCall = foundCommand->second.first;
	functionCall(parsedCommand, foundCommand->second.second);
}

void Console::RegisterCommand(string identifier, function<void(vector<string>, void*)> command, void* data) {
	commands.insert(pair<string, pair<function<void(vector<string>, void*)>, void*>>(identifier, make_pair(command, data)));
}

void Console::AddToPreviousCommandList() {
	if (previousCommands.size() == 5) {
		previousCommands.erase(previousCommands.begin());
	}
	previousCommands.push_back(currentCommand);
}

void Console::Update() {
	if (Window::GetKeyboard()->KeyPressed(KEYBOARD_TILDE)) {
		consoleOpen = !consoleOpen;
		currentCommand = "";
	}

	if (consoleOpen) {
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_0)) {
			currentCommand += "0";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_1)) {
			currentCommand += "1";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_2)) {
			currentCommand += "2";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_3)) {
			currentCommand += "3";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_4)) {
			currentCommand += "4";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_5)) {
			currentCommand += "5";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_6)) {
			currentCommand += "6";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_7)) {
			currentCommand += "7";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_8)) {
			currentCommand += "8";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_9)) {
			currentCommand += "9";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_A)) {
			currentCommand += "a";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_B)) {
			currentCommand += "b";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_C)) {
			currentCommand += "c";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_D)) {
			currentCommand += "d";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_E)) {
			currentCommand += "e";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_F)) {
			currentCommand += "f";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_G)) {
			currentCommand += "g";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_H)) {
			currentCommand += "h";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_I)) {
			currentCommand += "i";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_J)) {
			currentCommand += "j";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_K)) {
			currentCommand += "k";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_L)) {
			currentCommand += "l";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_M)) {
			currentCommand += "m";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_N)) {
			currentCommand += "n";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_O)) {
			currentCommand += "o";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_P)) {
			currentCommand += "p";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_Q)) {
			currentCommand += "q";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_R)) {
			currentCommand += "r";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_S)) {
			currentCommand += "s";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_T)) {
			currentCommand += "t";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_U)) {
			currentCommand += "u";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_V)) {
			currentCommand += "v";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_W)) {
			currentCommand += "w";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_X)) {
			currentCommand += "x";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_Y)) {
			currentCommand += "y";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_Z)) {
			currentCommand += "z";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_MINUS)) {
			currentCommand += "-";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_SPACE)) {
			currentCommand += " ";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_RETURN)) {
			HandleCommand();
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_BACK)) {
			currentCommand.pop_back();
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_UP)) {
			previousCommandIndex--;

			if (previousCommandIndex < 0) {
				previousCommandIndex = previousCommands.size() - 1;
			}

			if (!empty(previousCommands)) {
				currentCommand = previousCommands[previousCommandIndex];
			}
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_DOWN)) {
			previousCommandIndex++;

			if (previousCommandIndex > previousCommands.size() - 1) {
				previousCommandIndex = 0;
			}

			if (!empty(previousCommands)) {
				currentCommand = previousCommands[previousCommandIndex];
			}
		}

		for (int i = 0; i < previousCommands.size(); i++)
		{
			Debug::AddStringToDebugMenu(previousCommands[i]);
		}
		Debug::AddStringToDebugMenu(currentCommand);
	}
}
