#pragma once
#pragma once
#include "GameTechRenderer.h"
#include "GameScene.h"
#include "HUD.h"
#include "HUDObject.h"
#include "Animation.h"
#include "Player.h"
#include "Resource.h"
#include "CameraControl.h"


namespace NCL {
	namespace CSC8503 {

		class LevelScene : public GameScene {
		public:
			LevelScene(Game* g, bool& quitGame);
			~LevelScene();
			void GenerateResources();
			void LoadWorld();
			
		protected:
			void ResetWorld();

		};
	}
}
