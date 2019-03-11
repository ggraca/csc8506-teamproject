#pragma once

#include "GameTechRenderer.h"
#include "Scene.h"

class Game {
public:
	Game();
	~Game();
	void Update(float dt);
	void InitialiseAssets();

private:
	GameTechRenderer* renderer;
	Scene* currentScene;
};