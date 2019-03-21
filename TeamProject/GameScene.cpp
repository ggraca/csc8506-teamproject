#include "GameScene.h"
#include "Light.h"


GameScene::GameScene() : Scene() {
	debugMenu = DebugMenu();
	console = Console();
	RegisterConsoleCommands();

	GameObject* light = new GameObject("Directional Light");
	//light->GetTransform().SetLocalOrientation(Quaternion::EulerAnglesToQuaternion(30.0f, 60.0f, 0.0f));
	light->GetTransform().SetWorldPosition(Vector3(0.0f, 100.0f, 0.0f));
	light->AddComponent<Light*>(new Light(LightType::Point, Vector4(1.0f, 1.0f, 1.0f, 1.0f), 800.0f, 1.0f));
	world->Instantiate(light);
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
