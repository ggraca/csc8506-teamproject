#pragma once
#include "Scene.h"
#include "MenuEntry.h"
#include "Game.h"

class MenuScene : public Scene
{
public:
	MenuScene(Game* g);
	~MenuScene();

	vector<MenuEntry> MenuEntries(int index) { return menuEntries[index]; }
	virtual void Update(float dt);
	void MenuUpdate(float dt);
	//void NavigationUpdate(bool& quitGame, bool& showPauseMenu, CAudioEngine* audio, int& currentMenuPath, float dt, GameTechRenderer* renderer);
	void ShowMenu();

protected:
	map<int, std::vector<MenuEntry>> menuEntries;
	int menuPathIndex;
	Game* game;
};

