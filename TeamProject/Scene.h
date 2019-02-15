#pragma once
#include "GameTechRenderer.h"

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

			void SetBulletPhysicsParameters(btCollisionShape* Shape, const Vector3& position, float mass, float restitution, float friction, Quaternion orientation = Quaternion::AxisAngleToQuaterion(Vector3(0, 0, 0), 0));
			//GameObject*
			void AddSphereToWorld(OGLTexture* sphereTex, const Vector3& position, float radius, float mass = 10.0f, float restitution = 0.9, float friction = 0.4, Vector4 colour = Vector4(1,1,1,1));
			void AddCubeToWorld(OGLTexture* cubeTex, const Vector3& position, const Quaternion& orient, Vector3 dimension, float mass = 10.0f, float restitution = 0.9, float friction = 0.4, Vector4 colour = Vector4(1, 1, 1, 1));
			/*GameObject* AddCylinderToWorld(const Vector3& position, Vector3 dimensions, float mass = 10.0f);*/

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
			OGLTexture* brickTex = nullptr;
			OGLTexture* woodTex	= nullptr;
			OGLTexture* wood2Tex = nullptr;
			OGLTexture* grassTex	= nullptr;
			OGLTexture* ballTex	= nullptr;
			OGLTexture* dogTex = nullptr;
			OGLTexture* tempTex = nullptr;
			OGLShader*	basicShader = nullptr;
		};
	}
}
