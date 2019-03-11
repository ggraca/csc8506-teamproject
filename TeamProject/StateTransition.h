#pragma once
#include <functional>

namespace NCL {
	namespace CSC8503 {

		class State;
		class PhysicsScene;
		class StateTransition
		{
		public:
			virtual bool CanTransition() const = 0;

			State* GetDestinationState()  const {
				return destinationState;
			}

			State* GetSourceState() const {
				return sourceState;
			}



		protected:
			State * sourceState;
			State * destinationState;

		};

		template <class T, class U>
		class GenericTransition : public StateTransition
		{
		public:
			typedef bool(*GenericTransitionFunc)(T, U);
			GenericTransition(GenericTransitionFunc f, T testData, U otherData, State* srcState, State* destState) :
				dataA(testData), dataB(otherData)
			{
				func = f;
				sourceState = srcState;		//
				destinationState = destState;
			}
			~GenericTransition() {}

			virtual bool CanTransition() const override {
				if (func) {
					return func(dataA, dataB);
				}
				return false;
			}

			/*State* GetDestinationState()  const {
				return incomingState;
			}*/

			static bool GreaterThanTransition(T dataA, U dataB) {
				return dataA > dataB;
			}

			static bool LessThanTransition(T dataA, U dataB) {
				return dataA < dataB;
			}

			static bool EqualsTransition(T dataA, U dataB) {
				return dataA == dataB;
			}

			static bool NotEqualsTransition(T dataA, U dataB) {
				return dataA != dataB;
			}

		protected:
			GenericTransitionFunc  func;
			T dataA;
			U dataB;

		};




		template <class T, class U>
		class NextLvlTransition : public StateTransition {
		public:
			typedef std::function <void(PhysicsScene*)> NextLvlFunc;

			typedef bool(*LevelTransitionFunc)(T, U);



			NextLvlTransition(LevelTransitionFunc f, T testData, U otherData, State* srcState, State* destState, NextLvlFunc fc, PhysicsScene* someClass)
				: dataA(testData), dataB(otherData)
			{
				func = f;
				func2 = fc;
				sourceState = srcState;
				destinationState = destState;
				funcClass = someClass;
			}
			~NextLvlTransition() {}

			virtual bool CanTransition() const override {
				if (func) {
					bool canTransition = func(dataA, dataB);

					if (canTransition) func2(funcClass);

					return canTransition;
				}
				return false;
			}

			static bool HasHoledOut(T dataA, U dataB) {
				return (*dataA)->GetHoleOutState() == dataB;
			}

		protected:
			LevelTransitionFunc func;
			T dataA;
			U dataB;

			NextLvlFunc func2;
			PhysicsScene* funcClass;
		};


		template <class T, class U>
		class ShowMenuTransition : public StateTransition {
		public:

			typedef bool(*MenuTransitionFunc)(T, U);


			ShowMenuTransition(MenuTransitionFunc f, T testData, U otherData, State* srcState, State* destState, PhysicsScene* someClass)
				: dataA(testData), dataB(otherData)
			{
				func = f;
				sourceState = srcState;
				destinationState = destState;
				funcClass = someClass;

			}
			~ShowMenuTransition() {}

			virtual bool CanTransition() const override {
				if (func) {
					return func(dataA, dataB);

				}
				return false;
			}

			static bool EqualsTransition(T dataA, U dataB) {
				return dataA == dataB;
			}

		protected:
			MenuTransitionFunc func;
			T dataA;
			U dataB;

			PhysicsScene* funcClass;
		};

	}
}

