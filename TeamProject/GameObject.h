#pragma once
#include "Transform.h"
#include "LayerAndTag.h"
#include <vector>
#include <map>
#include "TypeId.h"

using std::vector;


namespace NCL {
	namespace CSC8503 {
		class Component;
		class ScriptObject;
		class GameWorld;
		

		class GameObject	{
		public:
			GameObject(std::string name = "");
			virtual ~GameObject();

			void ClearComponents();

			bool IsActive() const {
				return isActive;
			}

			const Transform& GetConstTransform() const {
				return transform;
			}

			Transform& GetTransform() {
				return transform;
			}

			const std::string& GetName() const {
				return name;
			}

			void SetName(std::string s) {
				name = s;
			}

			void SetLayer(LayerAndTag::ObjectLayer l)
			{
				this->layer = l;
			}

			LayerAndTag::ObjectLayer GetLayer() const
			{
				return this->layer;
			}

			void SetTag(LayerAndTag::Tags t)
			{
				this->tag = t;
			}

			LayerAndTag::Tags GetTag() const
			{
				return this->tag;
			}

			void SetIsAddedToWorld(bool status) { isAddedToWorld = status; }
			bool GetIsAddedToWorld() const { return isAddedToWorld; }

			bool CompareTag(LayerAndTag::Tags t)
			{
				return (this->tag == t);
			}

			bool CompareTag(const GameObject& other)
			{

				return (this->tag == other.tag);
			}

			virtual void OnCollisionBegin(GameObject* otherObject);


			virtual void OnCollisionEnd(GameObject* otherObject);


			void SetParent(GameObject * parent);
			

			bool IsParent(const Transform* transform);
			

			void AddChild(GameObject * child);
			
			template<class T>
			void AddComponent(Component * obj)
			{
				if (!obj) { return; }

				if (dynamic_cast<ScriptObject*>(obj)) { AddScript(dynamic_cast<ScriptObject*>(obj)); }

				int index = TypeId::GetTypeId(typeid(T));

				if (components[index])
				{
					delete components[index];
				}

				components[index] = obj;
			}

			template<class T>
			void RemoveComponent()
			{
				int index = TypeId::GetTypeId(typeid(T));

				if (dynamic_cast<ScriptObject*>(T)) { RemoveScript<T>(); }
				delete components[index];
				components.erase(index);
			}

			template<class T>
			T GetComponent()
			{
				int index = TypeId::GetTypeId(typeid(T));
				return dynamic_cast<T>(components[index]);
			}

			void SetUpInitialScripts();
			bool HasOtherScriptsAttached() { return (scripts.size() > 0); }
			void UpdateComponents(float dt);
			void LateUpdateAttachedScripts(float dt);
			void CallOnCollisionEnterForScripts(GameObject * otherObject);
			void CallOnCollisionEndForScripts(GameObject * otherObject);

			bool HasComponentsAttached() { return (components.size() > 0); }

			static void SetGameWorld(GameWorld * world);
			static GameObject * Find(std::string name);
			static GameObject * FindGameObjectWithTag(LayerAndTag::Tags tag);
			static vector<GameObject *> FindGameObjectsWithTag(LayerAndTag::Tags tag);
			static vector<GameObject*> GetChildrenOfObject(GameObject* obj);
			static vector<GameObject*> GetChildrenOfObject(GameObject* obj, LayerAndTag::Tags tag);
			static  void Destroy(GameObject * obj);
			static void AddObjectToWorld(GameObject * obj);
			static void AddObjectToWorld(GameObject * obj,GameObject * parent);
			static GameObject * GetMainCamera();

			vector<GameObject*> collidingObjects;

		protected:

			static GameWorld *gameWorld;
			Transform			transform;
			LayerAndTag::ObjectLayer  layer;
			LayerAndTag::Tags   tag;
			std::vector<ScriptObject*> scripts;
			std::map<int, Component*> components;

			bool	isActive;
			bool	isAddedToWorld;
			std::string	name;

		private:

			void AddScript(ScriptObject* obj);

			template<class T>
			void RemoveScript()
			{

				for (int i = 0; i < scripts.size();i++)
				{
					if (dynamic_cast<T>(scripts[i]))
					{
						delete scripts[i];
						scripts.erase(scripts.begin() + i);

						return;
					}
				}
			}
		};
	}
}
