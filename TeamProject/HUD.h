#pragma once
#include "GameTechRenderer.h"
#include "../Common/TextureLoader.h"
//#include "../Plugins/OpenGLRendering/OGLTexture.h"
using namespace NCL;
class HUD
{
	public:
		HUD() {}
			
		~HUD() {}

		void Update(const float dt, GameTechRenderer* renderer);
		
		int ammo = 100;
		int hp = 100;
	protected:
};