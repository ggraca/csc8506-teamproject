#include "../Common/Window.h"

#include "../GameTechCommon/StateMachine.h"
#include "../GameTechCommon/StateTransition.h"
#include "../GameTechCommon/State.h"

#include "../GameTechCommon/GameServer.h"
#include "../GameTechCommon/GameClient.h"

#include "../GameTechCommon/NavigationGrid.h"

#include "TutorialGame.h"
#include "Coursework.h"
#include "NetworkedGame.h"

#include <time.h>

using namespace NCL;
using namespace CSC8503;

/*

The main function should look pretty familar to you!
We make a window, and then go into a while loop that repeatedly
runs our 'game' until we press escape. Instead of making a 'renderer'
and updating it, we instead make a whole game, and repeatedly update that,
instead.

This time, we've added some extra functionality to the window class - we can
hide or show the

*/
int main() {
	Window*w = Window::CreateGameWindow("CSC8503 Game technology!", 1280, 720);

	if (!w->HasInitialised()) {
		return -1;
	}

	srand(time(NULL));

	w->ShowOSPointer(false);
	w->LockMouseToWindow(true);

	// TutorialGame* g = new TutorialGame();
	Coursework* g = new Coursework();

	while (w->UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)) {
		float dt = w->GetTimer()->GetTimeDelta() / 1000.0f;

		if (dt > 1.0f) {
			continue; //must have hit a breakpoint or something to have a 1 second frame time!
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_PRIOR)) {
			w->ShowConsole(true);
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_NEXT)) {
			w->ShowConsole(false);
		}

		if (g->isServer) w->SetTitle("Server");
		else if (g->me != nullptr) w->SetTitle(g->me->name);
		else w->SetTitle("Connecting...");

		g->UpdateGame(dt);
	}
	delete g;
	Window::DestroyGameWindow();
}
