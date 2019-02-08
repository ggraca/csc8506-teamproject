#pragma once
#include "Transform.h"
#include "CollisionVolume.h"

#include "PhysicsObject.h"
#include "RenderObject.h"
#include "NetworkObject.h"
#include "LayerAndTag.h"

#include <vector>

using std::vector;

namespace NCL {
	namespace CSC8503 {
		class NetworkObject;

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

			bool CompareTag(LayerAndTag::Tags t)
			{
				return (this->tag == t);
			}

			bool CompareTag(const GameObject& other)
			{
				
				return (this->tag == other.tag);
			}

			virtual void OnCollisionBegin(GameObject* otherObject) {
				
			}

			virtual void OnCollisionEnd(GameObject* otherObject) {
				
			}

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
			
			virtual void Awake() { std::cout << "Awake is called for:" << name << std::endl; }
			virtual void Start(){ std::cout << "Start is called for:" << name << std::endl; }
			virtual void Update(){ std::cout << "Update is called for:" << name << std::endl; }
			virtual void LateUpdate(){ std::cout << "LateUpdate is called for:" << name << std::endl; }

			
			void AddScript(GameObject* obj);

			template<class T>
			void RemoveScript()
			{

				for (int i = 0; i < scripts.size();i++)
				{
					if (dynamic_cast<T>(scripts[i]))
					{
						std::cout << "Here" << std::endl;

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

			bool HasOtherScriptsAttached() { return (scripts.size() > 0); }

			void UpdateAttachedScripts();
			void LateUpdateAttachedScripts();
			
		protected:
			Transform			transform;

			CollisionVolume*	boundingVolume;
			PhysicsObject*		physicsObject;
			RenderObject*		renderObject;
			NetworkObject*		networkObject;
			LayerAndTag::ObjectLayer  layer;
			LayerAndTag::Tags   tag;
			std::vector<ScriptObject*> scripts;
			

			bool	isActive;
			string	name;	
			
		};

		class ScriptObject
		{
		protected:
			
			
		};

		class Player :  virtual public GameObject
		{
		public:
			virtual ~Player() {}

			void Awake ()override { std::cout << "Overriden awake is called for:" << name << std::endl; }
			void Start()override { std::cout << "Overriden Start is called for:" << name << std::endl; }
			void Update()override { std::cout << "Overriden Update is called for:" << name << std::endl; }
			void LateUpdate()override { std::cout << "Overriden LateUpdate is called for:" << name << std::endl; }
		};
		
}
}
