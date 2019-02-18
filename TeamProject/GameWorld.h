#pragma once
#include <vector>
#include <string>
#include "../TeamProject/LayerAndTag.h"

using namespace std;

class BulletPhysics;

namespace NCL {
		class Camera;
	namespace CSC8503 {
		class GameObject;
		class Constraint;

		class GameWorld	{
		public:
			GameWorld();
			~GameWorld();

			GameObject * Find(string name);
			GameObject * FindGameObjectWithTag(LayerAndTag::Tags tag);
			vector<GameObject *> FindGameObjectsWithTag(LayerAndTag::Tags tag);
			void Destroy(GameObject * obj);

			void Clear();
			void ClearAndErase();

			void UpdateGameObjects(float dt);
			void LateUpdateGameObjects(float dt);
			void AddGameObject(GameObject* o);
			void CallInitialObjectFunctions(NCL::CSC8503::GameObject * o);
			void AddGameObject(GameObject* o, const GameObject* parent);
			void RemoveGameObject(GameObject* o);

			Camera* GetMainCamera() const {
				return mainCamera;
			}

			virtual void UpdateWorld(float dt);

			void GetObjectIterators(
				std::vector<GameObject*>::const_iterator& first,
				std::vector<GameObject*>::const_iterator& last) const;

			vector<GameObject*> GetChildrenOfObject(const GameObject* obj);
			vector<GameObject*> GetChildrenOfObject(const GameObject* obj, LayerAndTag::Tags tag);
      
			int GetObjectCount();

			void SetLayering(LayerAndTag layer)
			{
				this->layering = layer;
			}

			LayerAndTag GetLayering() const
			{
				return this->layering;
			}

			void SetPhysics(BulletPhysics* bulletPhysics)
			{
				physics = bulletPhysics;
			}

		protected:
			void UpdateTransforms();
			void UpdateQuadTree();

			std::vector<GameObject*> gameObjects;

			Camera* mainCamera;
			LayerAndTag layering;
			BulletPhysics* physics;
		};
	}
}

