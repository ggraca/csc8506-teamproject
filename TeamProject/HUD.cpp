#include "HUD.h"

void HUD::Update(const float dt, GameTechRenderer* renderer)
{
	renderer->DrawHUDString("PlayerName", Vector2(20, renderer->GetRendererHeight() - 25), Vector4(0, 0, 0, 1), 0.5f);
	renderer->DrawHUDString("Health:" + to_string(hp), Vector2(20, renderer->GetRendererHeight() - 55), Vector4(0, 0, 0, 1), 0.5f);
	renderer->DrawHUDString("Ammo:" + to_string(ammo), Vector2(renderer->GetRendererWidth() - 250, renderer->GetRendererHeight() - 25), Vector4(0, 0, 0, 1), 0.5f);
}