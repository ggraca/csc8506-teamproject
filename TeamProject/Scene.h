#pragma once
#include "HUD.h"
#include "Debug.h"
#include "../Common/Camera.h"
#include "AudioEngine.h"
#include "BulletPhysics.h"

class Material;

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
			void InitialiseAssets();

			virtual void InitWorld();

			CAudioEngine* audio;

			GameWorld*		world;

			bool useGravity;

			float		forceMagnitude;

			OGLMesh*	cubeMesh	= nullptr;
			OGLMesh*	sphereMesh	= nullptr;
			OGLMesh*	cylinderMesh = nullptr;
			OGLMesh*	coneMesh = nullptr;
			TextureBase* basicTex	= nullptr;
			TextureBase* woodTex	= nullptr;
			TextureBase* grassTex	= nullptr;
			TextureBase* ballTex	= nullptr;

			TextureBase* brickTex = nullptr;
			TextureBase* dogTex = nullptr;
			TextureBase* dogsTex = nullptr;
			TextureBase* tempTex = nullptr;
			TextureBase* cubeMap = nullptr;
			TextureBase* pbrWoodDiff = nullptr;
			TextureBase* pbrWoodBump = nullptr;
			TextureBase* pbrWoodSpec = nullptr;
			TextureBase* pbrWoodMet = nullptr;
			ShaderBase*	pbrShader = nullptr;
			Material*   basicMaterial = nullptr;
			Material* floorMat = nullptr;
		};
	}
}
