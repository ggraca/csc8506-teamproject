#pragma once
#pragma once
#include "GameTechRenderer.h"
#include "Scene.h"
#include "DebugMenu.h"
#include "HUD.h"
#include "HUDObject.h"
#include "Animation.h"
#include "Player.h"
#include "Resource.h"
#include "CameraControl.h"


namespace NCL {
	namespace CSC8503 {

		class LevelScene : public Scene {
		public:
			LevelScene();
			~LevelScene();
			void LoadWorld();
			
		protected:
			void ResetWorld();
		};
	}
}
