#pragma once
#include "Component.h"

using namespace NCL;
using namespace CSC8503;

namespace NCL
{
	namespace CSC8503
	{
		class Component;
		class GameObject;
		class ScriptObject:virtual public Component
		{
		public:

			ScriptObject();
			ScriptObject(GameObject * go);

			virtual ~ScriptObject();


			virtual void Awake();
			virtual void Start();
			virtual void Update(float dt);
			virtual void LateUpdate(float dt);
			virtual void OnCollisionBegin(GameObject* otherObject);
			virtual void OnCollisionEnd(GameObject* otherObject);

		protected:
			GameObject * gameObject;
		};
	}
}


