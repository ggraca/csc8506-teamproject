#include "Menu.h"


MenuEntry Menu::AddMenuEntry(int index, string title, bool selected)
{
	return MenuEntry(index, title, selected);
}

Menu::Menu()
{
	//Main Menu
	menuEntries[0].push_back(AddMenuEntry(0, "Resume Game", true));
	menuEntries[0].push_back(AddMenuEntry(1, "Settings", false));
	menuEntries[0].push_back(AddMenuEntry(2, "Quit Game", false));

	//Settings
	menuEntries[1].push_back(AddMenuEntry(0, "Controls", true));
	menuEntries[1].push_back(AddMenuEntry(1, "Audio Settings", false));
	menuEntries[1].push_back(AddMenuEntry(2, "Graphics Settings", false));
	menuEntries[1].push_back(AddMenuEntry(3, "Back", false));
	
	//Audio Settings
	menuEntries[2].push_back(AddMenuEntry(0, "Volume Up", true));
	menuEntries[2].push_back(AddMenuEntry(1, "Volume Down", false));
	menuEntries[2].push_back(AddMenuEntry(2, "Back", false));
	
}


Menu::~Menu()
{
}

void Menu::Update(CAudioEngine* audio, int& currentMenuPath, float dt, GameTechRenderer* renderer)
{
	menuPathIndex = currentMenuPath;
	if (Window::GetKeyboard()->KeyPressed(KEYBOARD_UP)) {
		int nextMenuEntry;
		for (unsigned int i = 0; i < menuEntries[menuPathIndex].size(); i++)
		{
			if (menuEntries[menuPathIndex][i].selected)
			{
				if (i == 0) nextMenuEntry = 0;
				else nextMenuEntry = i - 1;
				menuEntries[menuPathIndex][nextMenuEntry+1].selected = false;
				menuEntries[menuPathIndex][nextMenuEntry].selected = true;
				return;
			}
		}

		
	}
	if (Window::GetKeyboard()->KeyPressed(KEYBOARD_DOWN)) {
		int nextMenuEntry;
		for (unsigned int i = 0; i < menuEntries[menuPathIndex].size(); i++)
		{
			if (menuEntries[menuPathIndex][i].selected)
			{
				if (i == menuEntries[menuPathIndex].size()-1) nextMenuEntry = menuEntries[menuPathIndex].size()-1;
				else nextMenuEntry = i + 1;
				menuEntries[menuPathIndex][nextMenuEntry - 1].selected = false;
				menuEntries[menuPathIndex][nextMenuEntry].selected = true;
				return;
			}
		}
	}

	if (Window::GetKeyboard()->KeyPressed(KEYBOARD_RETURN)) {
		if (menuPathIndex == 0 && menuEntries[0][0].selected)
		{
			//Resume game
		}
		else if (menuPathIndex == 0 && menuEntries[0][1].selected)
		{
			//Go to settings
			currentMenuPath = 1;
		}
		else if (menuPathIndex == 0 && menuEntries[0][2].selected)
		{
			//Quit game;
		}
		else if (menuPathIndex == 1 && menuEntries[1][0].selected)
		{
			//Go to Controls;
		}
		else if (menuPathIndex == 1 && menuEntries[1][1].selected)
		{
			currentMenuPath = 2;
		}
		else if (menuPathIndex == 1 && menuEntries[1][2].selected)
		{
			//Go to Graphics Settings;
		}
		else if (menuPathIndex == 1 && menuEntries[1][3].selected)
		{
			//Go to back to main menu;
			currentMenuPath = 0;
		}
		else if (menuPathIndex == 2 && menuEntries[2][0].selected)
		{
			//Increase volume
		}
		else if (menuPathIndex == 2 && menuEntries[2][1].selected)
		{
			//Decrease volume
		}
		else if (menuPathIndex == 2 && menuEntries[2][2].selected)
		{
			currentMenuPath = 1;
		}
	}
}
void Menu::ShowMenu(GameTechRenderer* renderer)
{
	float offset = 50.0f;
	for (MenuEntry me : MenuEntries(menuPathIndex))
	{
		Vector4 colour;
		if (me.selected) colour = Vector4(1, 0, 0, 1);
		else colour = Vector4(0, 0, 1, 1);
		renderer->DrawString(me.menuTitle, Vector2(50, Window::GetWindow()->GetScreenSize().y / 2 + offset), colour);
		offset -= 50.0f;
	}
}

