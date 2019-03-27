#pragma once
#include <iostream>
#include <string>

using namespace std;

struct MenuEntry
{
	int index;
	string menuTitle;
	bool selected;
	MenuEntry(int i, string title, bool sel) : index(i), menuTitle(title), selected(sel) {}
};

