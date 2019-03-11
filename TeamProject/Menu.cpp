#include "Menu.h"


MenuEntry Menu::AddMenuEntry(int index, string title, bool selected)
{
	return MenuEntry(index, title, selected);
}

Menu::Menu()
{
	menuEntries.push_back(AddMenuEntry(0, "Resume Game", true));
	menuEntries.push_back(AddMenuEntry(1, "Settings", false));
	menuEntries.push_back(AddMenuEntry(2, "Quit Game", false));
}


Menu::~Menu()
{
}

void Menu::Update(float dt, GameTechRenderer* renderer)
{
	if (Window::GetKeyboard()->KeyPressed(KEYBOARD_UP)) {
		int nextMenuEntry;
		for (unsigned int i = 0; i < menuEntries.size(); i++)
		{
			if (menuEntries[i].selected)
			{
				if (i == 0) nextMenuEntry = 0;
				else nextMenuEntry = i - 1;
			}
		}
		menuEntries[nextMenuEntry+1].selected = false;
		menuEntries[nextMenuEntry].selected = true;

		/*(if (menuEntries[1].selected)
		{
			menuEntries[1].selected = false;
			menuEntries[0].selected = true;
		}
		else if (menuEntries[2].selected)
		{
			menuEntries[2].selected = false;
			menuEntries[1].selected = true;
		}*/
	}
	if (Window::GetKeyboard()->KeyPressed(KEYBOARD_DOWN)) {
		if (menuEntries[0].selected)
		{
			menuEntries[0].selected = false;
			menuEntries[1].selected = true;
		}
		else if (menuEntries[1].selected)
		{
			menuEntries[1].selected = false;
			menuEntries[2].selected = true;
		}
	}
}

