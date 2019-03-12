#pragma once
#include "GameTechRenderer.h"
#include <string>
#include <vector>

using namespace std;

struct MenuEntry
{
	int index;
	string menuTitle;
	bool selected;
	MenuEntry(int i, string title, bool sel) : index(i), menuTitle(title), selected(sel) {}
};


class Menu
{
public:
	Menu();
	~Menu();
	MenuEntry Menu::AddMenuEntry(int index, string title, bool selected);
	vector<MenuEntry> MenuEntries(int index) { return menuEntries[index]; }
	void Update(CAudioEngine* audio, int& currentMenuPath, float dt, GameTechRenderer* renderer);
	void ShowMenu(GameTechRenderer* renderer);
	
protected:
	map<int, std::vector<MenuEntry>> menuEntries;
	int menuPathIndex;
};

