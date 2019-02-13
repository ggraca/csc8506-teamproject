#include <time.h>

#include "../Common/Window.h"
#include "ExampleScene.h"

using namespace NCL;
using namespace CSC8503;

int main() {
  srand(time(NULL));

	Window*w = Window::CreateGameWindow("Team Project!", 1280, 720);
	if (!w->HasInitialised()) return -1;

	w->ShowOSPointer(false);
	w->LockMouseToWindow(true);
    w->ShowConsole(true);

	ExampleScene* g = new ExampleScene();

	while (w->UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)) {
		float dt = w->GetTimer()->GetTimeDelta() / 1000.0f;

		if (dt > 1.0f) {
			continue; //must have hit a breakpoint or something to have a 1 second frame time!
		}
		if (g->GetInputManager()->IsButtonPressed(InputManager::FORWARD)) {cout<<"Forward pressed"<<endl;}

		g->UpdateGame(dt);
	}

	delete g;
	Window::DestroyGameWindow();
}
