#include "DebugMenu.h"
#include "Debug.h"

using namespace NCL;
using namespace NCL::CSC8503;

DebugMenu::DebugMenu()
{
}


DebugMenu::~DebugMenu()
{
}

void DebugMenu::Update(const float dt, GameTechRenderer* renderer) {
	if (debugMenuOpen) {
		Debug::AddStringToDebugMenu("Graphics: ");
		calculateNewFPS++;
		if (calculateNewFPS >= 10) {
			frameTime = dt;
			calculateNewFPS = 0;
		}
		Debug::AddStringToDebugMenu("FPS: " + std::to_string((int)(1 / frameTime)) + "." + std::to_string((int)(10 / frameTime) % 10));
		Debug::AddStringToDebugMenu("Screen Size: " + std::to_string(renderer->GetRendererWidth())
			+ "x" + std::to_string(renderer->GetRendererHeight()));
		Debug::AddStringToDebugMenu("Verts: " + std::to_string(renderer->GetVertsDrawn()));
		Debug::AddStringToDebugMenu("Shadow Casters: " + std::to_string(renderer->GetShadowCasters()));
	}
}
