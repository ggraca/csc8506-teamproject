#include "ExampleScene.h"
#include "../GameTechCommon/GameWorld.h"
#include "../Plugins/OpenGLRendering/OGLMesh.h"
#include "../Plugins/OpenGLRendering/OGLShader.h"
#include "../Plugins/OpenGLRendering/OGLTexture.h"
#include "../Common/TextureLoader.h"

#include "../GameTechCommon/PositionConstraint.h"
#include "../Common/Assets.h"

#include <fstream>


using namespace NCL;
using namespace CSC8503;


ExampleScene::ExampleScene() : Scene() {
  physics->SetGravity(Vector3(0, -4, 0));
  physics->UseGravity(false);
  world->ShuffleConstraints(true);
  world->ShuffleObjects(true);

  Window::GetWindow()->ShowOSPointer(false);
  Window::GetWindow()->LockMouseToWindow(true);

  ResetWorld();
  debugMenu = DebugMenu();
}

void ExampleScene::ResetWorld() {
  world->ClearAndErase();
  physics->Clear();

  // Floor
  AddFloorToWorld(Vector3(200, 0, 200));
}

ExampleScene::~ExampleScene() {
}

void ExampleScene::UpdateGame(float dt) {
  world->GetMainCamera()->UpdateCamera(dt);
  world->UpdateWorld(dt);

  renderer->Update(dt);
  physics->Update(dt);

  Debug::FlushRenderables();
  debugMenu.Update(dt, renderer);

  renderer->Render();
}

void ExampleScene::DebugScene(float dt) {

	if (Window::GetKeyboard()->KeyPressed(KEYBOARD_TILDE)) {
		consoleOpen = !consoleOpen;
		consoleCommand = "";
	}

	if (consoleOpen) {
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_0)) {
			consoleCommand += "0";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_1)) {
			consoleCommand += "1";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_2)) {
			consoleCommand += "2";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_3)) {
			consoleCommand += "3";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_4)) {
			consoleCommand += "4";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_5)) {
			consoleCommand += "5";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_6)) {
			consoleCommand += "6";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_7)) {
			consoleCommand += "7";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_8)) {
			consoleCommand += "8";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_9)) {
			consoleCommand += "9";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_A)) {
			consoleCommand += "a";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_B)) {
			consoleCommand += "b";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_C)) {
			consoleCommand += "c";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_D)) {
			consoleCommand += "d";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_E)) {
			consoleCommand += "e";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_F)) {
			consoleCommand += "f";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_G)) {
			consoleCommand += "g";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_H)) {
			consoleCommand += "h";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_I)) {
			consoleCommand += "i";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_J)) {
			consoleCommand += "j";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_K)) {
			consoleCommand += "k";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_L)) {
			consoleCommand += "l";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_M)) {
			consoleCommand += "m";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_N)) {
			consoleCommand += "n";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_O)) {
			consoleCommand += "o";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_P)) {
			consoleCommand += "p";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_Q)) {
			consoleCommand += "q";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_R)) {
			consoleCommand += "r";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_S)) {
			consoleCommand += "s";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_T)) {
			consoleCommand += "t";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_U)) {
			consoleCommand += "u";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_V)) {
			consoleCommand += "v";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_W)) {
			consoleCommand += "w";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_X)) {
			consoleCommand += "x";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_Y)) {
			consoleCommand += "y";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_Z)) {
			consoleCommand += "z";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_SPACE)) {
			consoleCommand += " ";
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_RETURN)) {
			HandleCommand();
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_BACK)) {
			consoleCommand.pop_back();
		}

		Debug::AddStringToDebugMenu(consoleCommand);
	}
}

void ExampleScene::HandleCommand() {
	std::cout << consoleCommand << std::endl;
	consoleCommand = "";
}