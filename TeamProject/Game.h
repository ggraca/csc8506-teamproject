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

private:
	Scene* currentScene;
	GameTechRenderer* renderer;
	NetworkManager* network;
};