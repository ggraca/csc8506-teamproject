#pragma once
#include "GameTechRenderer.h"
#include "MenuEntry.h"

#include <vector>




class PauseMenu
{
public:
	PauseMenu();
	~PauseMenu();
	vector<MenuEntry> MenuEntries(int index) { return menuEntries[index]; }
	void Update(bool& quitGame, bool& showPauseMenu, CAudioEngine* audio, int& currentMenuPath, float dt, GameTechRenderer* renderer);
	void ShowMenu(GameTechRenderer* renderer);
	
protected:
	map<int, std::vector<MenuEntry>> menuEntries;
	int menuPathIndex;
};

