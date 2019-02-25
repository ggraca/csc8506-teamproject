#pragma once
#include "GameTechRenderer.h"
#include "HUD.h"
#include "Debug.h"
#include "../Common/Camera.h"
#include "BulletPhysics.h"


class Material;

namespace NCL {
	namespace CSC8503 {
		class Scene		{
		public:
			Scene();
			~Scene();

			virtual void UpdateGame(float dt);
			BulletPhysics*		physics; //TODO Make protected again?
		protected:
			void InitialiseAssets();

			void InitCamera();
			void UpdateKeys();

			virtual void InitWorld();

			void InitMixedGridWorld(const Vector3& position, int numRows, int numCols, float rowSpacing, float colSpacing);

			GameObject* AddSphereToWorld(const Vector3& position, float radius, float mass = 10.0f, float restitution = 0.9, float friction = 0.4);
			GameObject* AddCubeToWorld(const Vector3& position, const Quaternion& orient, Vector3 dimension, float mass = 10.0f, float restitution = 0.9, float friction = 0.4);

			GameTechRenderer*	renderer;

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
