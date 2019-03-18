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
	bool QuitGame() { return quitGame; }
	void ChangeCurrentScene(Scene* newScene, GameTechRenderer* r);

private:
	Scene* currentScene;
	GameTechRenderer* renderer;
	bool quitGame = false;
	NetworkManager* network;
};