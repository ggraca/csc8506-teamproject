#pragma once
#include "GameTechRenderer.h"

#include "../GameTechCommon/PhysicsSystem.h"
#include "..\Plugins\Bullet\src\btBulletDynamicsCommon.h"
#include "..\GameTechCommon\BulletPhysics.h"

namespace NCL {
	namespace CSC8503 {
		class Scene		{
		public:
			Scene();
			/*Scene(float g);*/
			~Scene();

			virtual void UpdateGame(float dt);

			

		protected:
			void InitialiseAssets();

			void InitCamera();
			void UpdateKeys();

			virtual void InitWorld();

			/*
			These are some of the world/object creation functions I created when testing the functionality
			in the module. Feel free to mess around with them to see different objects being created in different
			test scenarios (constraints, collision types, and so on).
			*/
			//void InitSphereGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, float radius);
			//void InitMixedGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing);
			//void InitCubeGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, const Vector3& cubeDims);
			//void InitSphereCollisionTorqueTest();
			//void InitCubeCollisionTorqueTest();
			//void InitSphereAABBTest();
			//void InitGJKWorld();
			//void BridgeConstraintTest();
			//void SimpleGJKTest();
			//void SimpleAABBTest();
			//void SimpleAABBTest2();

			bool SelectObject();
			void MoveSelectedObject();

			void SetBulletPhysicsParameters(btCollisionShape* Shape, const Vector3& position, float inverseMass);

			GameObject* AddFloorToWorld(const Vector3& position);
			GameObject* AddSphereToWorld(const Vector3& position, float radius, float inverseMass = 10.0f);
			GameObject* AddCubeToWorld(const Vector3& position, Vector3 dimensions, float inverseMass = 10.0f);

			GameTechRenderer*	renderer;
			PhysicsSystem*		physics;
			GameWorld*		world;

			bool useGravity;
			bool inSelectionMode;

			float		forceMagnitude;

			GameObject* selectionObject = nullptr;

			OGLMesh*	cubeMesh	= nullptr;
			OGLMesh*	sphereMesh	= nullptr;
			OGLTexture* basicTex	= nullptr;
			OGLTexture* woodTex	= nullptr;
			OGLTexture* grassTex	= nullptr;
			OGLTexture* ballTex	= nullptr;
			OGLShader*	basicShader = nullptr;
		};
	}
}
