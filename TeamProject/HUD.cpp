#include "HUD.h"

void HUD::Update(const float dt, GameTechRenderer* renderer)
{
	renderer->DrawString("PlayerName",							Vector2(20, renderer->GetRendererHeight() - 25),									Vector4(0, 0, 0, 1), 0.5f);
	renderer->DrawString("Health:" + to_string(hp),				Vector2(20, renderer->GetRendererHeight() - 55),									Vector4(0, 0, 0, 1), 0.5f);
	renderer->DrawString("Resources:" + to_string(resources),	Vector2(20, renderer->GetRendererHeight() - 85),									Vector4(0, 0, 0, 1), 0.5f);

	if (timestepCounter < 3.0f) renderer->DrawString(ready, Vector2(renderer->GetRendererWidth() / 2 - 330, renderer->GetRendererHeight() - 200), Vector4(0, 0, 0, 1), 1.5f);
	else if (timestepCounter > 3.0f && timestepCounter < 5.0f) renderer->DrawString(readySet, Vector2(renderer->GetRendererWidth() / 2 - 330, renderer->GetRendererHeight() - 200), Vector4(0, 0, 0, 1), 1.5f);
	else if (timestepCounter > 5.0f && timestepCounter < 7.0f) renderer->DrawString(readySetGo, Vector2(renderer->GetRendererWidth() / 2 - 330, renderer->GetRendererHeight() - 200), Vector4(0, 0, 0, 1), 1.5f);
	
	if (timestepCounter < 8.0f) timestepCounter += dt;
}


