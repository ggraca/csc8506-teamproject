#pragma once


namespace NCL
{
	namespace CSC8503
	{
		class GameObject;
		class Component
		{
		public:
			Component();
			Component(GameObject * go);
			virtual ~Component();
			virtual void Update(float dt);
			void SetGameObject(GameObject * go);
			GameObject * GetGameObject() const;

		protected:
			GameObject * gameObject = nullptr;
		};
	}
}


