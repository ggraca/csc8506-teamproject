#pragma once

#include "Scene.h"
#include "GameTechRenderer.h"
#include "NetworkManager.h"
#include "Console.h"
#include "DebugMenu.h"


class Game {
public:
	Game();
	~Game();
	void Update(float dt);
	void InitialiseAssets();
	void QuitGame() { quitGame = true; }
	bool& QuittingGame() { return quitGame; }
	void ChangeCurrentScene(Scene* newScene, GameTechRenderer* r, bool server);

private:
	Console console;
	Scene* currentScene;
	GameTechRenderer* renderer;
	bool quitGame = false;
#ifdef _WIN32
	NetworkManager* network;
#endif
};