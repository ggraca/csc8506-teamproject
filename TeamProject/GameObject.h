#pragma once
#include "Transform.h"
#include "../GameTechCommon/CollisionVolume.h"
#include "../GameTechCommon/PhysicsObject.h"
#include "../GameTechCommon/RenderObject.h"
#include "LayerAndTag.h"
//#include "GameWorld.h"


#include <vector>

using std::vector;


namespace NCL {
	namespace CSC8503 {
		class NetworkObject;
		class InputManager;
		class ScriptObject;
		class GameWorld;

		class GameObject	{
		public:
			GameObject(string name = "");
			virtual ~GameObject();

			void ClearScripts();

			void SetBoundingVolume(CollisionVolume* vol) {
				boundingVolume = vol;
			}

			const CollisionVolume* GetBoundingVolume() const {
				return boundingVolume;
			}

			bool IsActive() const {
				return isActive;
			}

			const Transform& GetConstTransform() const {
				return transform;
			}

			Transform& GetTransform() {
				return transform;
			}

			RenderObject* GetRenderObject() const {
				return renderObject;
			}

			PhysicsObject* GetPhysicsObject() const {
				return physicsObject;
			}

			NetworkObject* GetNetworkObject() const {
				return networkObject;
			}

			void SetRenderObject(RenderObject* newObject) {
				renderObject = newObject;
			}

			void SetPhysicsObject(PhysicsObject* newObject) {
				physicsObject = newObject;
			}

			const string& GetName() const {
				return name;
			}

			void SetName(string s) {
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




			bool InsideAABB(const Vector3& pos, const Vector3& halfSize);

			void SetParent(const GameObject * parent)
			{
				if (parent)
				{
					this->GetRenderObject()->GetTransform()->SetParent(parent->GetRenderObject()->GetTransform());
					parent->GetRenderObject()->GetTransform()->AddChild(this->GetRenderObject()->GetTransform());
				}
				else
				{
					if (this->GetRenderObject()->GetTransform()->GetParent() != nullptr)
					{
						this->GetRenderObject()->GetTransform()->GetParent()->RemoveChild(this->GetRenderObject()->GetTransform());
					}

					this->GetRenderObject()->GetTransform()->SetParent(nullptr);
				}
			}

			bool IsParent(const Transform* transform)
			{
				return (this->GetRenderObject()->GetTransform()->GetParent() == transform);
			}

			void AddChild(GameObject * child)
			{
				child->SetParent(this);
			}

			void AddScript(ScriptObject* obj);


			template<class T>
			void RemoveScript()
			{

				for (int i = 0; i < scripts.size();i++)
				{
					if (dynamic_cast<T>(scripts[i]))
					{
						delete scripts[i];
						scripts.erase(scripts.begin()+i);

						return;
					}
				}
			}

			template<class T>
			T GetScript()
			{
				for (auto& i : scripts)
				{
					if (dynamic_cast<T>(i))
					{
						return dynamic_cast<T>(i);
					}
				}

				return nullptr;
			}

			void SetUpInitialScripts();

			bool HasOtherScriptsAttached() { return (scripts.size() > 0); }

			void UpdateAttachedScripts(float dt);
			void LateUpdateAttachedScripts(float dt);


			static void SetGameWorld(GameWorld * world);
			static GameObject * Find(string name);
			static GameObject * FindGameObjectWithTag(LayerAndTag::Tags tag);
			static vector<GameObject *> FindGameObjectsWithTag(LayerAndTag::Tags tag);
			static vector<GameObject*> GetChildrenOfObject(const GameObject* obj);
			static vector<GameObject*> GetChildrenOfObject(const GameObject* obj, LayerAndTag::Tags tag);
			static  void Destroy(GameObject * obj);
			static void AddObjectToWorld(GameObject * obj);
			static void AddObjectToWorld(GameObject * obj,GameObject * parent);

		protected:
			static GameWorld *gameWorld;
			Transform			transform;
			CollisionVolume*	boundingVolume;
			PhysicsObject*		physicsObject;
			RenderObject*		renderObject;
			NetworkObject*		networkObject;
			LayerAndTag::ObjectLayer  layer;
			LayerAndTag::Tags   tag;
			std::vector<ScriptObject*> scripts;


			bool	isActive;
			bool	isAddedToWorld;
			string	name;

		};


		class ScriptObject
		{
		public:

			ScriptObject();


			ScriptObject(GameObject * go);
			ScriptObject(GameObject * go,InputManager* im);

			virtual ~ScriptObject();


			virtual void Awake() =0;
			virtual void Start()=0;
			virtual void Update(float dt)=0;
			virtual void LateUpdate(float dt)=0;
			virtual void OnCollisionBegin(GameObject* otherObject)=0;
			virtual void OnCollisionEnd(GameObject* otherObject)=0;

		protected:

			GameObject * gameObject;
			InputManager * inputManager;

		};

	}
}