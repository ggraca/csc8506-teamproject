#pragma once

#include "Scene.h"
#include "GameTechRenderer.h"
#include "NetworkManager.h"


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
	Scene* currentScene;
	GameTechRenderer* renderer;
	bool quitGame = false;
	NetworkManager* network;
};