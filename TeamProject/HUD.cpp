#include "HUD.h"

void HUD::Update(const float dt, GameTechRenderer* renderer)
{
	renderer->DrawString("PlayerName",							Vector2(20, renderer->GetRendererHeight() - 25),									Vector4(0, 0, 0, 1), 0.5f);
	renderer->DrawString("Health:" + to_string(hp),				Vector2(20, renderer->GetRendererHeight() - 55),									Vector4(0, 0, 0, 1), 0.5f);
	renderer->DrawString("Resources:" + to_string(resources),	Vector2(20, renderer->GetRendererHeight() - 85),									Vector4(0, 0, 0, 1), 0.5f);

	if (timestepCounter > 1.0f && timestepCounter < 3.0f) renderer->DrawString(ready, Vector2(renderer->GetRendererWidth() / 2 - 330, renderer->GetRendererHeight() - 200), Vector4(0, 0, 0, 1), 1.5f);
	else if (timestepCounter > 3.0f && timestepCounter < 5.0f) renderer->DrawString(readySet, Vector2(renderer->GetRendererWidth() / 2 - 330, renderer->GetRendererHeight() - 200), Vector4(0, 0, 0, 1), 1.5f);
	else if (timestepCounter > 5.0f && timestepCounter < 7.0f) renderer->DrawString(readySetGo, Vector2(renderer->GetRendererWidth() / 2 - 330, renderer->GetRendererHeight() - 200), Vector4(0, 0, 0, 1), 1.5f);
	
	if (timestepCounter < 8.0f) timestepCounter += dt;

	if (isGameOver)
	{
		GameOver(dt, renderer);
	}
	
}


void HUD::GameOver(const float dt, GameTechRenderer* renderer)
{
	
	if (gameOverCounter > 0.5f && gameOverCounter < 0.7f) renderer->DrawString(gameOver, Vector2(renderer->GetRendererWidth() / 2 - 300, renderer->GetRendererHeight() - 300), Vector4(0, 0, 0, 1), 1.0f);
	else if (gameOverCounter > 0.7f && gameOverCounter < 0.9f) renderer->DrawString(gameOver, Vector2(renderer->GetRendererWidth() / 2 - 300, renderer->GetRendererHeight() - 300), Vector4(0, 0, 0, 1), 1.1f);
	else if (gameOverCounter > 0.9f && gameOverCounter < 1.1f) renderer->DrawString(gameOver, Vector2(renderer->GetRendererWidth() / 2 - 300, renderer->GetRendererHeight() - 300), Vector4(0, 0, 0, 1), 1.2f);
	else if (gameOverCounter > 1.1f && gameOverCounter < 1.3f) renderer->DrawString(gameOver, Vector2(renderer->GetRendererWidth() / 2 - 300, renderer->GetRendererHeight() - 300), Vector4(0, 0, 0, 1), 1.3f);
	else if (gameOverCounter > 1.3f && gameOverCounter < 1.5f) renderer->DrawString(gameOver, Vector2(renderer->GetRendererWidth() / 2 - 300, renderer->GetRendererHeight() - 300), Vector4(0, 0, 0, 1), 1.4f);
	else if (gameOverCounter > 1.5f && gameOverCounter < 1.7f) renderer->DrawString(gameOver, Vector2(renderer->GetRendererWidth() / 2 - 300, renderer->GetRendererHeight() - 300), Vector4(0, 0, 0, 1), 1.5f);
	else if (gameOverCounter > 1.7f && gameOverCounter < 1.9f) renderer->DrawString(gameOver, Vector2(renderer->GetRendererWidth() / 2 - 300, renderer->GetRendererHeight() - 300), Vector4(0, 0, 0, 1), 1.6f);
	else if (gameOverCounter > 1.9f && gameOverCounter < 2.1f) renderer->DrawString(gameOver, Vector2(renderer->GetRendererWidth() / 2 - 300, renderer->GetRendererHeight() - 300), Vector4(0, 0, 0, 1), 1.7f);
	else if (gameOverCounter > 2.1f && gameOverCounter < 2.3f) renderer->DrawString(gameOver, Vector2(renderer->GetRendererWidth() / 2 - 300, renderer->GetRendererHeight() - 300), Vector4(0, 0, 0, 1), 1.8f);
	else if (gameOverCounter > 2.3f && gameOverCounter < 2.5f) renderer->DrawString(gameOver, Vector2(renderer->GetRendererWidth() / 2 - 300, renderer->GetRendererHeight() - 300), Vector4(0, 0, 0, 1), 1.9f);
	else if (gameOverCounter > 2.5f && gameOverCounter < 5.0f) renderer->DrawString(gameOver, Vector2(renderer->GetRendererWidth() / 2 - 300, renderer->GetRendererHeight() - 300), Vector4(0, 0, 0, 1), 2.0f);

	if (gameOverCounter < 5.1f) gameOverCounter += dt;
}


