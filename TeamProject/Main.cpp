#include <time.h>

#include "../Common/Window.h"
#include "PhysicsScene.h"
#include "ExampleScene.h"

using namespace NCL;
using namespace CSC8503;

int main(int argc, char** argv) {
	srand(unsigned(time(NULL)));

	Window*w = Window::CreateGameWindow("Team Project!", 1280, 720);
	//Window*w = Window::CreateGameWindow("Team Project!", 1920, 1200, true);
	if (!w->HasInitialised()) return -1;

	w->ShowOSPointer(false);
	w->LockMouseToWindow(true);
	w->ShowConsole(true);

	//Scene* scene = new ExampleScene();
	bool quitGame = false;

	Scene* scene = new PhysicsScene(quitGame);
	
	while (w->UpdateWindow() && !quitGame) {
		float dt = w->GetTimer()->GetTimeDelta() / 1000.0f;

		if (dt > 3.0f) {
			continue; //must have hit a breakpoint or something to have a 3 second frame time!
		}

		scene->UpdateGame(dt);
	}

	delete scene;
	Window::DestroyGameWindow();
}
