#pragma once
#include "GameTechRenderer.h"

#include "../GameTechCommon/BulletPhysics.h"

class Material;

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

			void SetBulletPhysicsParameters(btCollisionShape* Shape, const Vector3& position, float inverseMass, float restitution, float friction, Quaternion orientation = Quaternion::AxisAngleToQuaterion(Vector3(0, 0, 0), 0));

			GameObject* AddSphereToWorld(const Vector3& position, float radius, float inverseMass = 10.0f, float restitution = 0.9, float friction = 0.4);
			GameObject* AddCubeToWorld(const Vector3& position, const Quaternion& orient, Vector3 dimension, float inverseMass = 10.0f, float restitution = 0.9, float friction = 0.4);
			GameObject* AddFloorToWorld(const Vector3& position, const Quaternion& orient, Vector3 dimension, float inverseMass = 10.0f, float restitution = 0.9, float friction = 0.4);
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
