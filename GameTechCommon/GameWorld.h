#pragma once
#include <vector>
#include "Ray.h"
#include "CollisionDetection.h"
#include "QuadTree.h"
#include "LayerAndTag.h"


namespace NCL {
		class Camera;
		using Maths::Ray;
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

			void AddConstraint(Constraint* c);
			void RemoveConstraint(Constraint* c);

			Camera* GetMainCamera() const {
				return mainCamera;
			}

			void ShuffleConstraints(bool state) {
				shuffleConstraints = state;
			}

			void ShuffleObjects(bool state) {
				shuffleObjects = state;
			}

			bool Raycast(Ray& r, RayCollision& closestCollision, bool closestObject = false) const;

			virtual void UpdateWorld(float dt);

			void GetObjectIterators(
				std::vector<GameObject*>::const_iterator& first,
				std::vector<GameObject*>::const_iterator& last) const;

			vector<GameObject*> GetChildrenOfObject(const GameObject* obj);
			vector<GameObject*> GetChildrenOfObject(const GameObject* obj, LayerAndTag::Tags tag);
      
			int GetObjectCount();

			void GetConstraintIterators(
				std::vector<Constraint*>::const_iterator& first,
				std::vector<Constraint*>::const_iterator& last) const;

			void SetLayering(LayerAndTag layer)
			{
				this->layering = layer;
			}

			LayerAndTag GetLayering() const
			{
				return this->layering;
			}

		protected:
			void UpdateTransforms();
			void UpdateQuadTree();

			std::vector<GameObject*> gameObjects;

			std::vector<Constraint*> constraints;

			QuadTree<GameObject*>* quadTree;

			Camera* mainCamera;

			bool shuffleConstraints;
			bool shuffleObjects;
			LayerAndTag layering;
		};
	}
}

