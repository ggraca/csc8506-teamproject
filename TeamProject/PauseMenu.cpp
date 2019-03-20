#include "PauseMenu.h"




PauseMenu::PauseMenu()
{
	//Main Menu
	menuEntries[0].push_back(MenuEntry(0, "Resume Game", true));
	menuEntries[0].push_back(MenuEntry(1, "Settings", false));
	menuEntries[0].push_back(MenuEntry(2, "Quit Game", false));

	//Settings
	menuEntries[1].push_back(MenuEntry(0, "Controls", true));
	menuEntries[1].push_back(MenuEntry(1, "Audio Settings", false));
	menuEntries[1].push_back(MenuEntry(2, "Graphics Settings", false));
	menuEntries[1].push_back(MenuEntry(3, "Back", false));
	
	//Audio Settings
	menuEntries[2].push_back(MenuEntry(0, "Volume Up", true));
	menuEntries[2].push_back(MenuEntry(1, "Volume Down", false));
	menuEntries[2].push_back(MenuEntry(2, "Back", false));

	//Controls
	menuEntries[3].push_back(MenuEntry(0, "Back", true));
	menuEntries[3].push_back(MenuEntry(1, "W        - Forward", false));
	menuEntries[3].push_back(MenuEntry(2, "A        - Left", false));
	menuEntries[3].push_back(MenuEntry(3, "S        - Backward", false));
	menuEntries[3].push_back(MenuEntry(4, "D        - Right", false));
	menuEntries[3].push_back(MenuEntry(5, "Space    - Jump", false));
	menuEntries[3].push_back(MenuEntry(6, "Mouse 1  - Fire", false));
	
}


PauseMenu::~PauseMenu()
{
}

void PauseMenu::Update(bool& quitGame, bool& showPauseMenu, CAudioEngine* audio, int& currentMenuPath, float dt, GameTechRenderer* renderer)
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
			//Resume Game
			showPauseMenu = !showPauseMenu;
		}
		else if (menuPathIndex == 0 && menuEntries[0][1].selected)
		{
			//Go to settings
			currentMenuPath = 1;
		}
		else if (menuPathIndex == 0 && menuEntries[0][2].selected)
		{
			//Quit Game
			quitGame = true;
		}
		else if (menuPathIndex == 1 && menuEntries[1][0].selected)
		{
			//Go to Controls;
			currentMenuPath = 3;
		}
		else if (menuPathIndex == 1 && menuEntries[1][1].selected)
		{
			//Go to Audio Settings
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
			//Go back to Settings
			currentMenuPath = 1;
		}
		else if (menuPathIndex == 3 && menuEntries[3][0].selected)
		{
			//Go Back to Settings from Controls
			currentMenuPath = 1;
		}
	}
}
void PauseMenu::ShowMenu(GameTechRenderer* renderer)
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
