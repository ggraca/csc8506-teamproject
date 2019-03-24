#include "DebugMenu.h"
#include "Debug.h"

using namespace NCL;
using namespace NCL::CSC8503;

void DebugMenu::Update(const float dt, GameTechRenderer* renderer) {
	if (GetInstance().debugMenuOpen) {
		Debug::AddStringToDebugMenu("Graphics: ");
		GetInstance().calculateNewFPS++;
		if (GetInstance().calculateNewFPS >= 10) {
			GetInstance().frameTime = dt;
			GetInstance().calculateNewFPS = 0;
		}
		Debug::AddStringToDebugMenu("FPS: " + std::to_string((int)(1 / GetInstance().frameTime)) + "." + std::to_string((int)(10 / GetInstance().frameTime) % 10));
		Debug::AddStringToDebugMenu("Screen Size: " + std::to_string(renderer->GetRendererWidth())
			+ "x" + std::to_string(renderer->GetRendererHeight()));
		Debug::AddStringToDebugMenu("Verts: " + std::to_string(renderer->GetVertsDrawn()));
		Debug::AddStringToDebugMenu("Shadow Casters: " + std::to_string(renderer->GetShadowCasters()));

		auto functionTimes = GetInstance().timerManager.GetFunctionTimerMap();

		Debug::AddStringToDebugMenu("");
		Debug::AddStringToDebugMenu("Feature Timings:");
		for (auto i = functionTimes.begin(); i != functionTimes.end(); i++)
		{
			Debug::AddStringToDebugMenu(i->first + ": " + std::to_string(i->second) + " %" + std::to_string(i->second * 100 / GetInstance().frameTime));
		}
	}
}
