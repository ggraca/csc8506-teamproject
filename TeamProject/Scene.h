#pragma once
#include "GameTechRenderer.h"
#include "Debug.h"
#include "../Common/Camera.h"

#include "BulletPhysics.h"

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

			GameObject* AddSphereToWorld(const Vector3& position, float radius, float inverseMass = 10.0f, float restitution = 0.9, float friction = 0.4);
			GameObject* AddCubeToWorld(const Vector3& position, const Quaternion& orient, Vector3 dimension, float inverseMass = 10.0f, float restitution = 0.9, float friction = 0.4);
			GameObject* AddCylinderToWorld(const Vector3& position, const Quaternion& orient, Vector3 dimension, float inverseMass = 10.0f, float restitution = 0.9, float friction = 0.4);
			GameObject* AddConeToWorld(const Vector3& position, const Quaternion& orient, Vector3 dimension, float inverseMass = 10.0f, float restitution = 0.9, float friction = 0.4);

			GameTechRenderer*	renderer;
			
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
			OGLShader*	basicShader = nullptr;
		};
	}
}
