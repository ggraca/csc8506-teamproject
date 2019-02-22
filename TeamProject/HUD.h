#pragma once

#include "GameTechRenderer.h"
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
		int ammo = 100;
		int hp = 100;
	protected:
	
};