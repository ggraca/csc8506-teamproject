#pragma once

#include "Scene.h"
#include "GameTechRenderer.h"


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