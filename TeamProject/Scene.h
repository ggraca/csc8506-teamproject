#pragma once
#include "GameTechRenderer.h"

#include "../GameTechCommon/PhysicsSystem.h"
#include "../GameTechCommon/BulletPhysics.h"

namespace NCL {
	namespace CSC8503 {
		class Scene		{
		public:
			Scene();
			~Scene();

			virtual void UpdateGame(float dt);

		protected:
			void InitialiseAssets();

			void InitCamera();
			void UpdateKeys();

			virtual void InitWorld();

			bool SelectObject();
			void MoveSelectedObject();
			void InitMixedGridWorld(const Vector3& position, int numRows, int numCols, float rowSpacing, float colSpacing);

			void SetBulletPhysicsParameters(btCollisionShape* Shape, const Vector3& position, float inverseMass, Quaternion orientation = Quaternion::AxisAngleToQuaterion(Vector3(0, 0, 0), 0));

			GameObject* AddSphereToWorld(const Vector3& position, float radius, float inverseMass = 10.0f);
			GameObject* AddCubeToWorld(const Vector3& position, const Quaternion& orient, Vector3 dimension, float inverseMass = 10.0f);
			/*GameObject* AddCylinderToWorld(const Vector3& position, Vector3 dimensions, float inverseMass = 10.0f);*/

			GameTechRenderer*	renderer;
			BulletPhysics*		physics;
			GameWorld*		world;

			bool useGravity;
			bool inSelectionMode;

			float		forceMagnitude;

			GameObject* selectionObject = nullptr;

			OGLMesh*	cubeMesh	= nullptr;
			OGLMesh*	sphereMesh	= nullptr;
			OGLMesh*	cylinderMesh = nullptr;
			OGLTexture* basicTex	= nullptr;
			OGLTexture* woodTex	= nullptr;
			OGLTexture* grassTex	= nullptr;
			OGLTexture* ballTex	= nullptr;
			OGLShader*	basicShader = nullptr;
		};
	}
}
