#pragma once
#include "GameTechRenderer.h"
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