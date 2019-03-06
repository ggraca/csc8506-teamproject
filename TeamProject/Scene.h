#pragma once
#include "GameTechRenderer.h"
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
			CAudioEngine* audio;


			GameWorld*		world;

			bool useGravity;

			float		forceMagnitude;

			OGLMesh*	cubeMesh	= nullptr;
			OGLMesh*	sphereMesh	= nullptr;
			OGLMesh*	cylinderMesh = nullptr;
			OGLMesh*	coneMesh = nullptr;
			OGLTexture* basicTex	= nullptr;
			OGLTexture* woodTex	= nullptr;
			OGLTexture* grassTex	= nullptr;
			OGLTexture* ballTex	= nullptr;

			OGLTexture* brickTex = nullptr;
			OGLTexture* dogTex = nullptr;
			OGLTexture* dogsTex = nullptr;
			OGLTexture* tempTex = nullptr;
			OGLTexture* cubeMap = nullptr;
			OGLTexture* pbrWoodDiff = nullptr;
			OGLTexture* pbrWoodBump = nullptr;
			OGLTexture* pbrWoodSpec = nullptr;
			OGLTexture* pbrWoodMet = nullptr;
			OGLShader*	basicShader = nullptr;
			Material*   basicMaterial = nullptr;
			Material* floorMat = nullptr;

		};
	}
}
