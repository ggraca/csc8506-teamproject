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

		void Update(const float dt, GameTechRenderer* renderer, NetworkEntity* network);
		
		int resources = 0;
		int hp = 100;
		string ready = "Ready! ";
		string readySet = "Ready! Set! ";
		string readySetGo = "Ready! Set! Go!";
	protected:
		float timestepCounter = 0.0f;

};