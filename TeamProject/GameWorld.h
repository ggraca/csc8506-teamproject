#pragma once
#include <vector>
#include <string>
#include "LayerAndTag.h"
#include "CameraControl.h"
#include "RenderObject.h"
#include "NetworkEntity.h"

using namespace std;

class BulletPhysics;
class CAudioEngine;

namespace NCL {
		class Camera;
	namespace CSC8503 {
		class GameObject;
		class Constraint;

		class GameWorld	{
		public:
			GameWorld();
			void InitCamera();
			void SwitchToFPS();
			void SwitchToTPS();
			~GameWorld();

			GameObject * Find(string name);
			GameObject * FindGameObjectWithTag(LayerAndTag::Tags tag);
			vector<GameObject *> FindGameObjectsWithTag(LayerAndTag::Tags tag);

			void Clear();
			void ClearAndErase();
			
			void UpdateGameObjects(float dt);
			void LateUpdateGameObjects(float dt);
			void Instantiate(GameObject* o);
			void CallInitialObjectFunctions(NCL::CSC8503::GameObject * o);
			void Instantiate(GameObject* o,GameObject* parent);
			void RemoveGameObject(GameObject* o);

			GameObject* GetMainCamera() const {
				return mainCamera;
			}

			virtual void UpdateWorld(float dt);

			vector<GameObject*> GetChildrenOfObject(GameObject* obj);
			vector<GameObject*> GetChildrenOfObject(GameObject* obj, LayerAndTag::Tags tag);
      
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

			BulletPhysics* GetPhysics() {
				return physics;
      }
      
			void SetAudio(CAudioEngine* audioEngine) {
				audio = audioEngine;
			}

			CAudioEngine* GetAudio() {
				return audio;
			}

			void SetNetwork(NetworkEntity* ne)
			{
				network = ne;
			}

			GameObject* GetPlayerGameObject();
			vector<GameObject*> GetGameObjectList();

			void LateDestroy(GameObject * obj);
			void HandleObjectsToDestroy();
			void LateInstantiate(GameObject* obj);
			void HandleObjectsToInstantiate();

		protected:
			void UpdateTransforms();

			void Destroy(GameObject * obj);
			void RemoveCollisionsFromGameObject(GameObject * obj);


			std::vector<GameObject*> gameObjects;
			std::vector<GameObject*> objectsToDestroy;
			std::vector<GameObject*> objectsToInstantiate;

			GameObject* mainCamera;
			LayerAndTag layering;
			Vector3 cameraOffset;

			BulletPhysics* physics;
			CAudioEngine* audio;
			NetworkEntity* network;
		};
	}
}

