#include "Console.h"
#include "../Common/Utils.h"

using namespace NCL;

void Console::HandleCommand() {
	function<void(vector<string>, void*)> functionCall;

	if (currentCommand == "") {
		cout << __FUNCTION__ << " was called with an empty command!" << endl;
		return;
	}

	vector<string> parsedCommand = split_string(currentCommand, ' ');

	auto foundCommand = commands.find(parsedCommand[0]);
	AddToPreviousCommandList();
	previousCommandIndex = (int) previousCommands.size();
	currentCommand = "";

	if (foundCommand == commands.end()) {
		cout << __FUNCTION__ << " was called with invalid command!" << endl;
		return;
	}

	functionCall = foundCommand->second.first;
	functionCall(parsedCommand, foundCommand->second.second);
}

void Console::RegisterCommand(string identifier, function<void(vector<string>, void*)> command, void* data) {
	GetInstance().commands.insert(pair<string, pair<function<void(vector<string>, void*)>, void*>>(identifier, make_pair(command, data)));
}

void Console::AddToPreviousCommandList() {
	if (previousCommands.size() == 5) {
		previousCommands.erase(previousCommands.begin());
	}
	previousCommands.push_back(currentCommand);
}

void Console::Update() {
	if (GetInstance().consoleOpen) {
		if (!GetInstance().acceptingInput) {
			if (Window::GetKeyboard()->KeyPressed(KEYBOARD_RETURN)) {
				GetInstance().acceptingInput = true;
			}
		}
		else {
			if (Window::GetKeyboard()->KeyPressed(KEYBOARD_0)) {
				 GetInstance().currentCommand += "0";
			}
			if (Window::GetKeyboard()->KeyPressed(KEYBOARD_1)) {
				 GetInstance().currentCommand += "1";
			}
			if (Window::GetKeyboard()->KeyPressed(KEYBOARD_2)) {
				 GetInstance().currentCommand += "2";
			}
			if (Window::GetKeyboard()->KeyPressed(KEYBOARD_3)) {
				 GetInstance().currentCommand += "3";
			}
			if (Window::GetKeyboard()->KeyPressed(KEYBOARD_4)) {
				 GetInstance().currentCommand += "4";
			}
			if (Window::GetKeyboard()->KeyPressed(KEYBOARD_5)) {
				 GetInstance().currentCommand += "5";
			}
			if (Window::GetKeyboard()->KeyPressed(KEYBOARD_6)) {
				 GetInstance().currentCommand += "6";
			}
			if (Window::GetKeyboard()->KeyPressed(KEYBOARD_7)) {
				 GetInstance().currentCommand += "7";
			}
			if (Window::GetKeyboard()->KeyPressed(KEYBOARD_8)) {
				 GetInstance().currentCommand += "8";
			}
			if (Window::GetKeyboard()->KeyPressed(KEYBOARD_9)) {
				 GetInstance().currentCommand += "9";
			}
			if (Window::GetKeyboard()->KeyPressed(KEYBOARD_A)) {
				 GetInstance().currentCommand += "a";
			}
			if (Window::GetKeyboard()->KeyPressed(KEYBOARD_B)) {
				 GetInstance().currentCommand += "b";
			}
			if (Window::GetKeyboard()->KeyPressed(KEYBOARD_C)) {
				 GetInstance().currentCommand += "c";
			}
			if (Window::GetKeyboard()->KeyPressed(KEYBOARD_D)) {
				 GetInstance().currentCommand += "d";
			}
			if (Window::GetKeyboard()->KeyPressed(KEYBOARD_E)) {
				 GetInstance().currentCommand += "e";
			}
			if (Window::GetKeyboard()->KeyPressed(KEYBOARD_F)) {
				 GetInstance().currentCommand += "f";
			}
			if (Window::GetKeyboard()->KeyPressed(KEYBOARD_G)) {
				 GetInstance().currentCommand += "g";
			}
			if (Window::GetKeyboard()->KeyPressed(KEYBOARD_H)) {
				 GetInstance().currentCommand += "h";
			}
			if (Window::GetKeyboard()->KeyPressed(KEYBOARD_I)) {
				 GetInstance().currentCommand += "i";
			}
			if (Window::GetKeyboard()->KeyPressed(KEYBOARD_J)) {
				 GetInstance().currentCommand += "j";
			}
			if (Window::GetKeyboard()->KeyPressed(KEYBOARD_K)) {
				 GetInstance().currentCommand += "k";
			}
			if (Window::GetKeyboard()->KeyPressed(KEYBOARD_L)) {
				 GetInstance().currentCommand += "l";
			}
			if (Window::GetKeyboard()->KeyPressed(KEYBOARD_M)) {
				 GetInstance().currentCommand += "m";
			}
			if (Window::GetKeyboard()->KeyPressed(KEYBOARD_N)) {
				 GetInstance().currentCommand += "n";
			}
			if (Window::GetKeyboard()->KeyPressed(KEYBOARD_O)) {
				 GetInstance().currentCommand += "o";
			}
			if (Window::GetKeyboard()->KeyPressed(KEYBOARD_P)) {
				 GetInstance().currentCommand += "p";
			}
			if (Window::GetKeyboard()->KeyPressed(KEYBOARD_Q)) {
				 GetInstance().currentCommand += "q";
			}
			if (Window::GetKeyboard()->KeyPressed(KEYBOARD_R)) {
				 GetInstance().currentCommand += "r";
			}
			if (Window::GetKeyboard()->KeyPressed(KEYBOARD_S)) {
				 GetInstance().currentCommand += "s";
			}
			if (Window::GetKeyboard()->KeyPressed(KEYBOARD_T)) {
				 GetInstance().currentCommand += "t";
			}
			if (Window::GetKeyboard()->KeyPressed(KEYBOARD_U)) {
				 GetInstance().currentCommand += "u";
			}
			if (Window::GetKeyboard()->KeyPressed(KEYBOARD_V)) {
				 GetInstance().currentCommand += "v";
			}
			if (Window::GetKeyboard()->KeyPressed(KEYBOARD_W)) {
				 GetInstance().currentCommand += "w";
			}
			if (Window::GetKeyboard()->KeyPressed(KEYBOARD_X)) {
				 GetInstance().currentCommand += "x";
			}
			if (Window::GetKeyboard()->KeyPressed(KEYBOARD_Y)) {
				 GetInstance().currentCommand += "y";
			}
			if (Window::GetKeyboard()->KeyPressed(KEYBOARD_Z)) {
				 GetInstance().currentCommand += "z";
			}
      if (Window::GetKeyboard()->KeyPressed(KEYBOARD_MINUS)) {
			   GetInstance().currentCommand += "-";
		  }
			if (Window::GetKeyboard()->KeyPressed(KEYBOARD_SPACE)) {
				 GetInstance().currentCommand += " ";
			}
			if (Window::GetKeyboard()->KeyPressed(KEYBOARD_RETURN)) {
				GetInstance().HandleCommand();
				GetInstance().acceptingInput = false;
			}
			if (Window::GetKeyboard()->KeyPressed(KEYBOARD_BACK)) {
        if ( GetInstance().currentCommand != "") {
             GetInstance().currentCommand.pop_back();
        }
			}
      if (Window::GetKeyboard()->KeyPressed(KEYBOARD_UP)) {
		  GetInstance().previousCommandIndex--;

        if (GetInstance().previousCommandIndex < 0) {
			GetInstance().previousCommandIndex = (int)GetInstance().previousCommands.size() - 1;
        }

        if (!empty(GetInstance().previousCommands)) {
           GetInstance().currentCommand = GetInstance().previousCommands[GetInstance().previousCommandIndex];
        }
      }
      if (Window::GetKeyboard()->KeyPressed(KEYBOARD_DOWN)) {
		  GetInstance().previousCommandIndex++;

        if (GetInstance().previousCommandIndex > (int)GetInstance().previousCommands.size() - 1) {
			GetInstance().previousCommandIndex = 0;
        }

        if (!empty(GetInstance().previousCommands)) {
           GetInstance().currentCommand = GetInstance().previousCommands[GetInstance().previousCommandIndex];
        }
      }
      
      for (unsigned int i = 0; i < GetInstance().previousCommands.size(); i++)
      {
        Debug::AddStringToDebugMenu(GetInstance().previousCommands[i]);
      }
      
			if (GetInstance().indicatorTimer <= 30) {
				Debug::AddStringToDebugMenu( GetInstance().currentCommand + '|');
			}
			else {
				Debug::AddStringToDebugMenu( GetInstance().currentCommand);
				if (GetInstance().indicatorTimer >= 60) {
					GetInstance().indicatorTimer = 0;
				}
			}
		}
		GetInstance().indicatorTimer++;
	}
}
