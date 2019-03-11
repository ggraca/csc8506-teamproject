#pragma once

#include "AudioEngine.h"
#include "BulletPhysics.h"

namespace NCL {
	namespace CSC8503 {
		class Scene		{
		public:
			Scene();
			~Scene();

			virtual void UpdateGame(float dt) = 0;
			GameWorld* GetGameWorld() const { return world; }
			BulletPhysics* physics; //TODO Make protected again?

		protected:
			virtual void InitWorld();

			GameWorld* world;
			CAudioEngine* audio;
		};
	}
}
