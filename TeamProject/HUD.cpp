#include "HUD.h"

void HUD::Update(const float dt, GameTechRenderer* renderer, NetworkEntity* network)
{
	PlayerState ps = network->GetPlayerState();
	renderer->DrawString("PlayerName",							Vector2(20.0f, renderer->GetRendererHeight() - 25.0f),									Vector4(0.0f, 0.0f, 0.0f, 1.0f), 0.5f);
	renderer->DrawString("Health:" + to_string(ps.health),				Vector2(20.0f, renderer->GetRendererHeight() - 55.0f),									Vector4(0.0f, 0.0f, 0.0f, 1.0f), 0.5f);
	renderer->DrawString("Resources:" + to_string(ps.resources),	Vector2(20.0f, renderer->GetRendererHeight() - 85.0f),									Vector4(0.0f, 0.0f, 0.0f, 1.0f), 0.5f);

	if (timestepCounter < 3.0f) renderer->DrawString(ready, Vector2(renderer->GetRendererWidth() / 2 - 330.0f, renderer->GetRendererHeight() - 200.0f), Vector4(0.0f, 0.0f, 0.0f, 1.0f), 1.5f);
	else if (timestepCounter > 3.0f && timestepCounter < 5.0f) renderer->DrawString(readySet, Vector2(renderer->GetRendererWidth() / 2 - 330.0f, renderer->GetRendererHeight() - 200.0f), Vector4(0.0f, 0.0f, 0.0f, 1.0f), 1.5f);
	else if (timestepCounter > 5.0f && timestepCounter < 7.0f) renderer->DrawString(readySetGo, Vector2(renderer->GetRendererWidth() / 2 - 330.0f, renderer->GetRendererHeight() - 200.0f), Vector4(0.0f, 0.0f, 0.0f, 1.0f), 1.5f);
	
	if (timestepCounter < 8.0f) timestepCounter += dt;
}


