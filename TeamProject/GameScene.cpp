#include "GameScene.h"
#include "Light.h"


GameScene::GameScene() : Scene() {
	debugMenu = DebugMenu();
	console = Console();
	InitStateMachine();
	RegisterConsoleCommands();

	GameObject* light = new GameObject("Directional Light");
	light->GetTransform().SetLocalOrientation(Quaternion::EulerAnglesToQuaternion(90.0f, 0.0f, 0.0f));
	light->AddComponent<Light*>(new Light(LightType::Directional, Vector4(1.0f, 1.0f, 1.0f, 1.0f), 2000.0f, 1.0f));
	world->Instantiate(light);
}

GameScene::~GameScene()  {
	delete worldState;
	delete objectStateMachine;
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

	if (Window::GetKeyboard()->KeyPressed(KEYBOARD_ESCAPE)) {
		showPauseMenu = !showPauseMenu;
	}

	pauseMenu.Update(quitGame, showPauseMenu, audio, currentMenuPath, dt, renderer);
	objectStateMachine->Update();
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

void GameScene::ShowMenu()
{
	pauseMenu.ShowMenu(renderer);
}

void GameScene::UsedForMenu(void* data)
{
	//This doesn't do anything but helps with the menu toggle.
}

void GameScene::InitStateMachine()
{
	objectStateMachine = new StateMachine();

	//World
	std::function<void(GameScene*, void*)> WorldFunc = &GameScene::UsedForMenu;
	worldState = new WorldState<GameScene>(WorldFunc, (void *)& worldState, this);
	objectStateMachine->AddState(worldState);

	//Menu
	std::function<void(GameScene*)> menuFunc1 = &GameScene::ShowMenu;
	MenuState<GameScene>* mainMenu = new MenuState<GameScene>(menuFunc1, this);
	objectStateMachine->AddState(mainMenu);

	//Menu transition
	ShowMenuTransition<bool&, bool>* worldToMainMenu = new ShowMenuTransition <bool&, bool >(ShowMenuTransition <bool&, bool>::EqualsTransition, showPauseMenu, true, worldState, mainMenu, this);
	ShowMenuTransition<bool&, bool>* mainMenuToWorld = new ShowMenuTransition <bool&, bool>(ShowMenuTransition <bool&, bool>::EqualsTransition, showPauseMenu, false, mainMenu, worldState, this);
	objectStateMachine->AddTransition(worldToMainMenu);
	objectStateMachine->AddTransition(mainMenuToWorld);

}
