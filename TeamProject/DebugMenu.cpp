#include "DebugMenu.h"
#include "../GameTechCommon/Debug.h"

using namespace NCL;

DebugMenu::DebugMenu()
{
}


DebugMenu::~DebugMenu()
{
}

void DebugMenu::Update(const float dt, const GameTechRenderer& renderer) const {
	Debug::AddStringToDebugMenu("Graphics: ");
	Debug::AddStringToDebugMenu("FPS: " + std::to_string((int)(1 / dt)) + "." + std::to_string((int)(10 / dt) % 10));
	Debug::AddStringToDebugMenu("Screen Size: " + std::to_string(renderer.GetRendererWidth())
		+ "x" + std::to_string(renderer.GetRendererHeight()));
	Debug::AddStringToDebugMenu("Verts: " + std::to_string(renderer.GetVertsDrawn()));
	Debug::AddStringToDebugMenu("Shadow Casters: " + std::to_string(renderer.GetShadowCasters()));
}
