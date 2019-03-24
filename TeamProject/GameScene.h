#pragma once

#include "Scene.h"
#include "StateMachine.h"
#include "StateTransition.h"
#include "State.h"
#include "HUD.h"
#include "PauseMenu.h"


class GameScene : public Scene {
public:
	GameScene(bool& quitGame);
	virtual ~GameScene();

protected:
	void LateUpdate(float dt) override;
	void RegisterConsoleCommands();
	void InitStateMachine();
	void ShowMenu();

	HUD hud;

	PauseMenu pauseMenu;
	void UsedForMenu(void* data);
	bool showPauseMenu = false;
	WorldState<GameScene>* worldState;
	bool& quitGame;
	int currentMenuPath = 0;
	StateMachine* objectStateMachine;

};