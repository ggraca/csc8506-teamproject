#pragma once

#include "GameTechRenderer.h"
#include "NetworkManager.h"
#include "Console.h"
#include "DebugMenu.h"
#include "AudioEngine.h"

class Scene;
class Game {
public:
	Game();
	~Game();
	void Update(float dt);
	void InitialiseAssets();
	void QuitGame() { quitGame = true; }
	bool& QuittingGame() { return quitGame; }
	void ChangeCurrentScene(Scene* newScene, GameTechRenderer* r, bool server);
	CAudioEngine* GetAudio() const { return audio; }
	NetworkManager* GetNetwork() const { return network; }
	void CreateNetwork(bool server) { network = new NetworkManager(server); }

private:
	Scene* currentScene;
	GameTechRenderer* renderer;
	bool quitGame = false;
	NetworkManager* network;
	CAudioEngine* audio;
};