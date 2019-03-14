#include <time.h>

#include "../Common/Window.h"
#include "Game.h"

using namespace NCL;

int main(int argc, char** argv) {
	srand(unsigned(time(NULL)));

	Window*w = Window::CreateGameWindow("Team Project!", 1280, 720);
	//Window*w = Window::CreateGameWindow("Team Project!", 1920, 1200, true);
	if (!w->HasInitialised()) return -1;

	w->ShowOSPointer(false);
	w->LockMouseToWindow(true);
	w->ShowConsole(true);

	Game* game = new Game();

	Scene* scene = new PhysicsScene(quitGame);
	
	while (w->UpdateWindow() && !quitGame) {
		float dt = w->GetTimer()->GetTimeDelta() / 1000.0f;

		if (dt > 3.0f) {
			continue; //must have hit a breakpoint or something to have a 3 second frame time!
		}

		game->Update(dt);
	}

	delete game;
	Window::DestroyGameWindow();
}
