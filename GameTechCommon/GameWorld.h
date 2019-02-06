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

			void Clear();
			void ClearAndErase();

			void AddGameObject(GameObject* o);
			void GameWorld::AddGameObject(GameObject* o, const GameObject* parent);
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
			vector<GameObject*> GameWorld::GetChildrenOfObject(const GameObject* obj);
			vector<GameObject*> GameWorld::GetChildrenOfObject(const GameObject* obj, LayerAndTag::Tags tag);

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

