#pragma once

#include "Scene.h"
#include "GameTechRenderer.h"


class Game {
public:
	Game();
	~Game();
	void Update(float dt);
	void InitialiseAssets();
	bool QuitGame() { return quitGame; }

private:
	GameTechRenderer* renderer;
	Scene* currentScene;
	bool quitGame = false;
};