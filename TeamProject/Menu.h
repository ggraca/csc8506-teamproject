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
	vector<MenuEntry> MenuEntries() { return menuEntries; }
	void Update(float dt, GameTechRenderer* renderer);
	
protected:
	std::vector<MenuEntry> menuEntries;
};

