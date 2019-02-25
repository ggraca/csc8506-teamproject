#pragma once
#include "GameObject.h"

using namespace NCL;
using namespace CSC8503;

namespace NCL
{
	namespace CSC8503
	{
		class Component
		{
		public:
			Component();
			Component(GameObject * go);
			virtual ~Component();
			virtual void Update();

		protected:
			GameObject * gameObject;
		};
	}
}


