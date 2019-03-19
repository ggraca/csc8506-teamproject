#pragma once
#include "ExampleScene.h"
#include "LevelScene.h"
#include <vector>

using namespace NCL;
using namespace CSC8503;

namespace NCL
{
	namespace CSC8503
	{

		struct InputContainer;

		class InputManager
		{
		public:
			enum ActionButton
			{
				FORWARD,
				BACKWARD,
				LEFT,
				RIGHT,
				JUMP,
				DODGE,
				HIT,
				TOGGLE_GUN,
				TOGGLE_HAMMER,
				TOGGLE_SHIELD,
				CALL_AIR_STRIKE,
				MAX
			};

			InputManager(InputManager const&) = delete;
			void operator=(InputManager const&) = delete;

			static InputManager& GetInstance()
			{
				static InputManager instance;
				return instance;
			}


			static bool IsButtonPressed(ActionButton actionButton);
			static bool IsButtonHeld(ActionButton actionButton);
			static bool IsButtonDown(ActionButton actionButton);
			static bool IsDoubleClicked(ActionButton actionButton);

			static void InitializeButtonRelations();
			static void Dispose();

			static short GetInputBitsPressed();
			static short GetInputBitsDown();
			static short GetInputBitsHeld();
			static InputContainer* GenerateContainer(short s);
			static short GenerateShortFromContainer(InputContainer ic);


		private:
			InputManager() {};

			struct ButtonRelation
			{
				std::vector<NCL::KeyboardKeys> relatedKeyboardKeys;
				std::vector<NCL::MouseButtons> relatedMouseKeys;
			};


		protected:

			ButtonRelation * registeredActionButtons;

		};


		struct InputContainer
		{
			bool* inputs;

			InputContainer()
			{
				inputs = new bool[InputManager::ActionButton::MAX];
			}

			~InputContainer()
			{
				delete[] inputs;
			}
		};
	}
}

