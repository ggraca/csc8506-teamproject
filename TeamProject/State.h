#pragma once
#include <functional>
namespace NCL {
	namespace CSC8503 {
		class State {
		public:
			State() {}
			virtual ~State() {}
			virtual void Update() = 0; //Pure virtual base class
		};

		typedef void(*StateFunc)(void*);

		class GenericState : public State {
		public:
			GenericState(StateFunc someFunc, void* someData) {
				func = someFunc;
				funcData = someData;
			}
			virtual void Update() {
				if (funcData != nullptr) {
					func(funcData);
				}
			}

			/*std::string GetStateName() {
				return stateName;
			}*/

		protected:
			StateFunc func;
			void* funcData;
		};

		class PhysicsScene;
		typedef std::function <void(PhysicsScene*, void *)> LevelFunc;

		class LevelState : public State {
		public:
			LevelState(LevelFunc someFunc, void* someData, PhysicsScene* someClass) {
				func = someFunc;
				funcData = someData;
				funcClass = someClass;
			}
			virtual void Update() {
				if (funcData != nullptr) {
					func(funcClass, funcData);
				}
			}
		protected:
			LevelFunc func;
			void* funcData;
			PhysicsScene* funcClass;
		};

		class TutorialGame;

		typedef std::function <void(PhysicsScene*)> MenuFunc;

		class MenuState : public State {
		public:
			MenuState(MenuFunc someFunc, PhysicsScene* someClass)
			{
				func = someFunc;
				funcClass = someClass;

			}
			virtual void Update() {
				func(funcClass);
			}


		protected:
			MenuFunc func;

			PhysicsScene* funcClass;
		};


	}
}