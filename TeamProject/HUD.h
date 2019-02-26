#pragma once

#include "GameTechRenderer.h"
#include "../Common/TextureLoader.h"
//#include "../Plugins/OpenGLRendering/OGLTexture.h"
#include "../Common/Vector2.h"
#include "../Common/Vector4.h"

using namespace NCL;
using namespace NCL::CSC8503;

class HUD
{
	public:
		HUD() {}
			
		~HUD() {}

		void Update(const float dt, GameTechRenderer* renderer);
		void GameOver(const float dt, GameTechRenderer* renderer);
		
		int resources = 0;
		int hp = 100;

		//For testing
		bool isGameOver = false;
	protected:
		string ready = "Ready! ";
		string readySet = "Ready! Set! ";
		string readySetGo = "Ready! Set! Go!";
		string gameOver = "Game Over!";
		float timestepCounter = 0.0f;
		float gameOverCounter = 0.0f;


};