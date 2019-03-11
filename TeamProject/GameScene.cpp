#include "GameScene.h"


GameScene::GameScene() : Scene() {
	debugMenu = DebugMenu();
	console = Console();
	RegisterConsoleCommands();
}

void GameScene::LateUpdate(float dt) {
	//Might want moved into a seperate function that handles input
	if (Window::GetKeyboard()->KeyPressed(KEYBOARD_TILDE)) {
		console.Toggle();
		debugMenu.Toggle();
	}

	if (Window::GetKeyboard()->KeyPressed(KEYBOARD_V)) {
		world->SwitchToFPS();
	}
	if (Window::GetKeyboard()->KeyPressed(KEYBOARD_C)) {
		world->SwitchToTPS();
	}

	Debug::FlushRenderables();
	debugMenu.Update(dt, renderer);
	console.Update();
	hud.Update(dt, renderer);
}

void CommandSetCameraPosition(vector<string> commandParams, void* data) {
	float x = stof(commandParams[1]);
	float y = stof(commandParams[2]);
	float z = stof(commandParams[3]);

	GameWorld* world = (GameWorld*)data;
	world->GetMainCamera()->GetTransform().SetWorldPosition(Vector3(x, y, z));
}

void GameScene::RegisterConsoleCommands() {
	console.RegisterCommand("setcamerapos", CommandSetCameraPosition, world);
}