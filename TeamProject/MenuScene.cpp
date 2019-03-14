#include "MenuScene.h"
#include "PhysicsScene.h"

MenuScene::MenuScene(Game* g) : game(g)
{
	//Main Menu
	menuEntries[0].push_back(MenuEntry(0, "Create Game", true));
	menuEntries[0].push_back(MenuEntry(1, "Join Game", false));
	menuEntries[0].push_back(MenuEntry(2, "Settings", false));
	menuEntries[0].push_back(MenuEntry(3, "Quit Game", false));

	//Settings
	menuEntries[1].push_back(MenuEntry(0, "Controls", true));
	menuEntries[1].push_back(MenuEntry(1, "Audio Settings", false));
	menuEntries[1].push_back(MenuEntry(2, "Graphics Settings", false));
	menuEntries[1].push_back(MenuEntry(3, "Back", false));

	//Audio Settings
	menuEntries[2].push_back(MenuEntry(0, "Volume Up", true));
	menuEntries[2].push_back(MenuEntry(1, "Volume Down", false));
	menuEntries[2].push_back(MenuEntry(2, "Back", false));
}


MenuScene::~MenuScene()
{
}

void MenuScene::Update(float dt)
{
	ShowMenu();
	MenuUpdate(dt);
}

void MenuScene::MenuUpdate(float dt)
{
	//menuPathIndex = currentMenuPath;
	if (Window::GetKeyboard()->KeyPressed(KEYBOARD_UP)) {
		int nextMenuEntry;
		for (unsigned int i = 0; i < menuEntries[menuPathIndex].size(); i++)
		{
			if (menuEntries[menuPathIndex][i].selected)
			{
				if (i == 0) nextMenuEntry = 0;
				else nextMenuEntry = i - 1;
				menuEntries[menuPathIndex][nextMenuEntry + 1].selected = false;
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
				if (i == menuEntries[menuPathIndex].size() - 1) nextMenuEntry = menuEntries[menuPathIndex].size() - 1;
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
			//Create Game
			PhysicsScene* newScene = new PhysicsScene();
			game->ChangeCurrentScene(newScene, newScene->GetRenderer());			
		}
		else if (menuPathIndex == 0 && menuEntries[0][1].selected)
		{
			//Join Game
		}
		else if (menuPathIndex == 0 && menuEntries[0][2].selected)
		{
			menuPathIndex = 1;
		}
		else if (menuPathIndex == 0 && menuEntries[0][2].selected)
		{
			//Quit Game
		}
		else if (menuPathIndex == 1 && menuEntries[1][0].selected)
		{
			//Go to Controls;
		}
		else if (menuPathIndex == 1 && menuEntries[1][1].selected)
		{
			//Go to Audio Settings
			menuPathIndex = 2;
		}
		else if (menuPathIndex == 1 && menuEntries[1][2].selected)
		{
			//Go to Graphics Settings;
		}
		else if (menuPathIndex == 1 && menuEntries[1][3].selected)
		{
			//Go to back to main menu;
			menuPathIndex = 0;
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
			menuPathIndex = 1;
		}
	}
}

void MenuScene::ShowMenu()
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
